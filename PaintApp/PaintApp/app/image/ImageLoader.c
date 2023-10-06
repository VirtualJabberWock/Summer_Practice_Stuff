#include "ImageLoader.h"
#pragma once

DEFINE_OBJ_TYPE(ImageBitmap);
DEFINE_OBJ_TYPE(PixelBuffer);

static void DisposeImage(ImageBitmap* image) {
    DeleteObject(image->handle);
}

static void DisposePixelBuffer(PixelBuffer* buffer) {
    if(buffer->pixels != 0) free(buffer->pixels);
    buffer->pixels = 0;
}

PixelBuffer* NewPixelBuffer()
{
    PixelBuffer* buffer = calloc(1, sizeof(PixelBuffer));

    if (buffer == 0) {
        return debugMemError();
    }

    OBJECT_SUPER(PixelBuffer, buffer);

    OverrideObjectDispose(PixelBuffer, DisposePixelBuffer);

    buffer->pixels = 0;
    buffer->used = 0;
    buffer->wSize = 0;
    buffer->hSize = 0;

    return buffer;
}

int CopyPixelBuffer(PixelBuffer* destination, PixelBuffer* from)
{
    if (destination->pixels != 0)
        free(destination->pixels);

    destination->pixels = calloc(from->wSize * from->hSize, sizeof(UINT));
    destination->wSize = from->wSize;
    destination->hSize = from->hSize;

    if (destination->pixels == 0) {
        return debugMemError();
    }

    return memcpy(destination->pixels, from->pixels, from->wSize * from->hSize * sizeof(UINT));
}

void ClearAndResizePixelBuffer(PixelBuffer* buffer, int width, int height) {
    if (buffer->pixels != 0) {
        free(buffer->pixels);
    }
    buffer->pixels = calloc(width * height, sizeof(UINT));
    buffer->used = 1;
    buffer->wSize = width;
    buffer->hSize = height;
}

static ImageBitmap* CreateImage(HBITMAP bitmap, ImageFileFormat format) 
{
    ImageBitmap* image = calloc(1, sizeof(ImageBitmap));

    if (image == 0) {
        return debugMemError();
    }

    image->handle = bitmap;

    OBJECT_SUPER(ImageBitmap, image);

    OverrideObjectDispose(ImageBitmap, DisposeImage);


    BITMAP bm = { 0 };

    GetObject(image->handle, sizeof(bm), &bm);

    image->width = bm.bmWidth;
    image->height = bm.bmHeight;
    image->format = format;
}

ImageBitmap* ImageLoader_LoadBitmap(LPCWSTR filename, IWindowClass* window)
{

    HBITMAP temp = LoadImageW(
        window->context.hInst,
        filename,
        IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE
    );

    int err = GetLastError();
    if (err || temp == 0) {
        debugFatalErrorFormat("Error: %d [%x]", err, err);
    }

    return CreateImage(temp, IMAGE_FORMAT_BMP);
}

#include <stdio.h>
#include <windows.h>
#include "png.h"
#include <crtdbg.h>
#include <corecrt.h>

FILE* getReadBinaryFile(LPCWSTR path) {
    HANDLE hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return 0;
    }
    int nHandle = _open_osfhandle((long)hFile, _O_RDONLY);
    if (nHandle == -1) {
        CloseHandle(hFile);   //case 1
        return 0;
    }
    FILE* fp = _fdopen(nHandle, "rb");
    if (!fp) {
        CloseHandle(hFile);  //case 2
    }
    return fp;
}

long ReadPngData(LPCWSTR szPath, int* outWidth, int* outHeight, unsigned char** cbData)
{
    FILE* fp = NULL;
    long file_size = 0, pos = 0, mPos = 0;
    int color_type = 0, x = 0, y = 0, block_size = 0;

    png_infop info_ptr;
    png_structp png_ptr;
    png_bytep* row_point = NULL;

    HFILE _hfile_ = CreateFileW(szPath, READ_CONTROL, 0, NULL, OPEN_EXISTING, 0, 0);
    LPVOID _buffer_read;
    LPDWORD _bytes_read;
    int flag = ReadFile(_hfile_, _buffer_read, 5, _bytes_read, NULL);
    CloseHandle(_buffer_read);

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

    *outWidth = png_get_image_width(png_ptr, info_ptr);
    *outHeight = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    file_size = (*outWidth) * (*outHeight) * 4;
    *cbData = (unsigned char*)malloc(file_size);
    row_point = png_get_rows(png_ptr, info_ptr);

    block_size = color_type == 6 ? 4 : 3;

    for (x = 0; x < *outHeight; x++)
        for (y = 0; y < *outWidth * block_size; y += block_size)
        {
            (*cbData)[pos++] = row_point[x][y + 2];        
            (*cbData)[pos++] = row_point[x][y + 1];        
            (*cbData)[pos++] = row_point[x][y + 0];        
            (*cbData)[pos++] = row_point[x][y + 3];        
        }

    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    fclose(fp);

    return file_size;
}

ImageBitmap* ImageLoader_LoadPNG(LPCWSTR filename, IWindowClass* window)
{
    unsigned char* raw_data;
    int w = 0;
    int h = 0;
    ReadPngData(filename,&w, &h, &raw_data);
    HBITMAP temp = CreateBitmap(w,h, 32, 1, raw_data);
    return CreateImage(temp, IMAGE_FORMAT_PNG);
}

int isPointOnImage(ImageBitmap* image, int x, int y)
{
    return (x >= 0 && x <= image->width && y >= 0 && y <= image->height);
}

BOOL SaveHBITMAPToFile(HBITMAP hBitmap, LPCWSTR lpszFileName)
{
    HDC hDC;
    int iBits;
    WORD wBitCount;
    DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
    BITMAP Bitmap0;
    BITMAPFILEHEADER bmfHdr;
    BITMAPINFOHEADER bi;
    LPBITMAPINFOHEADER lpbi;
    HANDLE fh, hDib, hPal, hOldPal2 = NULL;
    hDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
    iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
    DeleteDC(hDC);
    if (iBits <= 1)
        wBitCount = 1;
    else if (iBits <= 4)
        wBitCount = 4;
    else if (iBits <= 8)
        wBitCount = 8;
    else
        wBitCount = 24;
    GetObject(hBitmap, sizeof(Bitmap0), (LPSTR)&Bitmap0);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = Bitmap0.bmWidth;
    bi.biHeight = Bitmap0.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = wBitCount;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrImportant = 0;
    bi.biClrUsed = 256;
    dwBmBitsSize = ((Bitmap0.bmWidth * wBitCount + 31) & ~31) / 8
        * Bitmap0.bmHeight;
    hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
    *lpbi = bi;

    hPal = GetStockObject(DEFAULT_PALETTE);
    if (hPal)
    {
        hDC = GetDC(NULL);
        hOldPal2 = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
        RealizePalette(hDC);
    }


    GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap0.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
        + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);

    if (hOldPal2)
    {
        SelectPalette(hDC, (HPALETTE)hOldPal2, TRUE);
        RealizePalette(hDC);
        ReleaseDC(NULL, hDC);
    }

    fh = CreateFileW(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (fh == INVALID_HANDLE_VALUE)
        return FALSE;

    bmfHdr.bfType = 0x4D42; // "BM"
    dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
    bmfHdr.bfSize = dwDIBSize;
    bmfHdr.bfReserved1 = 0;
    bmfHdr.bfReserved2 = 0;
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

    WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

    WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
    GlobalUnlock(hDib);
    GlobalFree(hDib);
    CloseHandle(fh);
    return TRUE;
}

void ImageLoader_Save(ImageBitmap* map, LPCWSTR filename, IWindowClass* window)
{
    if (map->format == IMAGE_FORMAT_BMP) {
        SaveHBITMAPToFile(map->handle, filename);
    }
}

void IL_Utils_SaveBitmap(HBITMAP* bmp, LPCWSTR filename) {
    SaveHBITMAPToFile(bmp, filename);
}
