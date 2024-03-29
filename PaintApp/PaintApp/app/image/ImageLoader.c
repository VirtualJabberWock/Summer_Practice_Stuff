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

#include <stdio.h>
#include <windows.h>

ImageBitmap* ImageLoader_LoadImage(LPCWSTR filename)
{
    int len = lstrlenW(filename);
    ImageFileFormat format = IMAGE_FORMAT_BMP;
    if (lstrcmpiW(filename + len - 4, L".png") == 0) {
        format = IMAGE_FORMAT_PNG;
    }
    if (lstrcmpiW(filename + len - 4, L".jpeg") == 0) {
        format = IMAGE_FORMAT_PNG;
    }
    HBITMAP temp = GDI_LoadImage(filename);
    return CreateImage(temp, format);
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
    GDI_SaveImage(filename, map->handle, map->format);
}

const char* ImageLodaer_GetFileFormatExtension(ImageBitmap* image)
{
    switch (image->format) {
    case IMAGE_FORMAT_BMP:
        return "bmp";
    case IMAGE_FORMAT_JPG:
        return "jpeg";
    case IMAGE_FORMAT_PNG:
        return "png";
    }
    return "bmp";
}

void IL_Utils_SaveBitmap(HBITMAP* bmp, LPCWSTR filename) {
    SaveHBITMAPToFile(bmp, filename);
}
