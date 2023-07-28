#include "ImageTransform.h"
#pragma once

#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) \
                                 | ((Color << 16) & 0xff0000)



#define MAKE_COL(r,g,b) ((r) + ((g) << 8) + ((b) << 16))

typedef (*PixelIterator) (UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo);

static HBITMAP CallIteratorOnPixels(HBITMAP hBmp, PixelIterator iterator, RECT* rect, int newW, int newH)
{
    HBITMAP RetBmp = NULL;
    if (hBmp)
    {
        HDC BufferDC = CreateCompatibleDC(NULL);  
        if (BufferDC)
        {
            HBITMAP hTmpBitmap = (HBITMAP)NULL;

            HGDIOBJ PreviousBufferObject = SelectObject(BufferDC, hBmp);

            HDC DirectDC = CreateCompatibleDC(NULL);
            if (DirectDC)
            {
                BITMAP bm;
                GetObject(hBmp, sizeof(bm), &bm);



                BITMAPINFO RGB32BitsBITMAPINFO;
                ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
                RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                RGB32BitsBITMAPINFO.bmiHeader.biWidth = (newW != 0) ? newW : bm.bmWidth;
                RGB32BitsBITMAPINFO.bmiHeader.biHeight = (newH != 0) ? newH : bm.bmHeight;
                RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
                RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;

                UINT* ptPixels;

                HBITMAP DirectBitmap = CreateDIBSection(DirectDC,
                    (BITMAPINFO*)&RGB32BitsBITMAPINFO,
                    DIB_RGB_COLORS,
                    (void**)&ptPixels,
                    NULL, 0);
                if (DirectBitmap)
                {
                    HGDIOBJ PreviousObject = SelectObject(DirectDC, DirectBitmap);
                    BitBlt(DirectDC, 0, 0,
                        bm.bmWidth, bm.bmHeight,
                        BufferDC, 0, 0, SRCCOPY);

                    int l, r, t, b;
                    if (rect != 0) {
                        l = min(rect->left, rect->right);
                        r = max(rect->left, rect->right);
                        t = min(rect->top, rect->bottom);
                        b = max(rect->top, rect->bottom);
                    }
                    else {
                        l = 0;
                        r = bm.bmWidth;
                        t = 0;
                        b = bm.bmHeight;
                    }

                    if (r > bm.bmWidth  || b > bm.bmHeight || l > bm.bmWidth || t > bm.bmHeight) {

                    }
                    else {

                        t = bm.bmHeight - t;
                        b = bm.bmHeight - b;

                        iterator(ptPixels, bm.bmWidth, bm.bmHeight, b, t, l, r);
                    }

                    SelectObject(DirectDC, PreviousObject);

                    RetBmp = DirectBitmap;
                }
                DeleteDC(DirectDC);
            }
            if (hTmpBitmap)
            {
                //SelectObject(hBmpDC, hBmp);
                DeleteObject(hTmpBitmap);
            }
            SelectObject(BufferDC, PreviousBufferObject);
            DeleteDC(BufferDC);
        }
    }
    return RetBmp;
}

static void InvertColorIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {

    for (int i = yFrom; i < yTo; i++) {
        for (int j = xFrom; j < xTo; j++) {
            pixels[i * width + j] = pixels[i * width + j] ^ 0x00ffffff;
        }
    }
}

static void BWColorIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {

    for (int i = yFrom; i < yTo; i++) {
        for (int j = xFrom; j < xTo; j++) {
            UINT pixel = pixels[i * width + j];
            UINT value = (GET_RED(pixel) + GET_GREEN(pixel) + GET_BLUE(pixel)) / 3;
            value = value / 16;
            value = value * 16;
            if (value == 240) value = 255;
            UINT color  = value + (value << 8) + (value << 16);
            pixels[i * width + j] = color;
        }
    }
}

typedef unsigned char byte;

static int brightRef = 0;

static void BrightIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {

    for (int i = yFrom; i < yTo; i++) {
        for (int j = xFrom; j < xTo; j++) {
            UINT pixel = pixels[i * width + j];
            int r = (GET_RED(pixel) + brightRef);
            int b = (GET_BLUE(pixel) + brightRef);
            int g = (GET_GREEN(pixel) + brightRef);
            r = max(min(r, 255), 0);
            g = max(min(g, 255), 0);
            b = max(min(b, 255), 0);
            pixels[i * width + j] = MAKE_COL(r, g, b);
        }
    }
}


#include <stdio.h>

static void N4_Blur_Iterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {


    char notFirst = 0;
    for (int k = 0; k <= 0; k++) {
        for (int i = yFrom + k; i < yTo - 2 - k; i += 1) {
            for (int j = xFrom + k; j < xTo - 2 - k; j += 1) {
                int pixel = pixels[i * width + j];
                int pixel1 = pixels[i * width + j + 1];
                int pixel2 = pixels[(i + 1) * width + j];
                int pixel3 = pixels[(i + 1) * width + j + 1];
                byte red = (GET_RED(pixel) + GET_RED(pixel1) + GET_RED(pixel2) + GET_RED(pixel3)) / 4;
                byte green = (GET_GREEN(pixel) + GET_GREEN(pixel1) + GET_GREEN(pixel2) + GET_GREEN(pixel3)) / 4;
                byte blue = (GET_BLUE(pixel) + GET_BLUE(pixel1) + GET_BLUE(pixel2) + GET_BLUE(pixel3)) / 4;
                UINT color = MAKE_COL(red, green, blue);
                pixels[i * width + j] = (notFirst) ? color : pixel;
                pixels[i * width + j + 1] = (notFirst) ? color : pixel1;
                pixels[(i + 1) * width + j] = (notFirst) ? color : pixel2;
                pixels[(i + 1) * width + j + 1] = (notFirst) ? color : pixel3;
                notFirst = 1;
            }
        }
    }
}

static int colorRef_0 = 0;

static void FillColorIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {

    for (int i = yFrom; i < yTo; i++) {
        for (int j = xFrom; j < xTo; j++) {
            pixels[i * width + j] = colorRef_0;
        }
    }
}

static char isVertical = 0;

static void MirrorIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {

    UINT* pixelsCopy = calloc(width * height, sizeof(UINT));
    if (pixelsCopy == 0) {
        return debugMemError();
    }
    for (int i = yFrom; i < yTo; i++) {
        int mI = yTo - i + yFrom - 1;
        for (int j = xFrom; j < xTo; j++) {
            int mJ = xTo - j + xFrom - 1;
            if (isVertical) {
                pixelsCopy[(i-yFrom) * width + (j-xFrom)] = pixels[mI * width + j];
            }
            else {
                pixelsCopy[(i - yFrom) * width + (j - xFrom)] = pixels[i * width + mJ];
            }
        }
    }
    for (int i = yFrom; i < yTo; i++) {
        int iPos = i * width;
        for (int j = xFrom; j < xTo; j++) {
            pixels[iPos + j] = pixelsCopy[(i - yFrom) * width + (j - xFrom)];
        }
    }
    free(pixelsCopy);
}

static PixelBuffer* bufferRef_0 = 0;

static void InsertBitmapIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {

    if (bufferRef_0 == 0) return;

    for (int i = yFrom; i < yTo; i++) {
        for (int j = xFrom; j < xTo; j++) {
            int bufI = i - yFrom;
            int bufJ = j - xFrom;
            pixels[i * width + j] = bufferRef_0->pixels[bufI * bufferRef_0->wSize + bufJ];
        }
    }
}

static void CopyToBufferIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {

    if (bufferRef_0 == 0) return;

    ClearAndResizePixelBuffer(bufferRef_0, abs(xTo - xFrom), abs(yTo - yFrom));

    for (int i = yFrom; i < yTo; i++) {
        for (int j = xFrom; j < xTo; j++) {
            int bufI = i - yFrom;
            int bufJ = j - xFrom;
            bufferRef_0->pixels[bufI * bufferRef_0->wSize + bufJ] = pixels[i * width + j];
        }
    }
}

static void PasteFromBufferIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {

    if (bufferRef_0 == 0) return;

    yFrom = yFrom - bufferRef_0->hSize;

    int yFromOriginal = height - yFrom;

    for (int i = yFrom; i < yFrom + bufferRef_0->hSize; i++) {
        for (int j = xFrom; j < xFrom+bufferRef_0->wSize; j++) {
            int bufI = i - yFrom;
            int bufJ = j - xFrom;
            if (j >= width) continue;
            if (j <= 0) continue;
            if (i >= height) continue;
            if (i < 0) continue;
            pixels[i * width + j] = bufferRef_0->pixels[bufI * bufferRef_0->wSize + bufJ];
        }
    }
}

static void PasteFromBufferColorIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {

    if (bufferRef_0 == 0) return;

    yFrom = yFrom - bufferRef_0->hSize;

    int yFromOriginal = height - yFrom;

    if (yFromOriginal > height) return;
    if (xFrom + bufferRef_0->wSize > width) return;

    for (int i = yFrom; i < yFrom+bufferRef_0->hSize; i++) {
        for (int j = xFrom; j < xFrom+bufferRef_0->wSize; j++) {
            int bufI = i - yFrom;
            int bufJ = j - xFrom;
            UINT pixel = pixels[i * width + j];
            UINT pixelBuf = bufferRef_0->pixels[bufI * bufferRef_0->wSize + bufJ];
            int r = GET_RED(pixelBuf);
            int b = GET_BLUE(pixelBuf);
            int g = GET_GREEN(pixelBuf);
            int r_ = GET_RED(pixel);
            int b_ = GET_BLUE(pixel);
            int g_ = GET_GREEN(pixel);
            r_ = min(r_ + (r/2)*2, 255);
            g_ = min(g_ + (g / 2) * 2, 255);
            b_ = min(b_ + (b / 2) * 2, 255);
            pixels[i * width + j] = r_ + (g_ << 8) + (b_ << 16);
        }
    }
}

void IT_ReplaceColor(ImageBitmap* image, COLORREF cOldColor, COLORREF cNewColor)
{
    //
}

void IT_InvertColor(ImageBitmap* image, RECT* region)
{
    HBITMAP map = CallIteratorOnPixels(image->handle, InvertColorIterator, region, 0, 0);
    DeleteObject(image->handle);
    image->handle = map;
}

void IT_BlackAndWhiteColor(ImageBitmap* image, RECT* region)
{
    HBITMAP map = CallIteratorOnPixels(image->handle, BWColorIterator, region, 0 , 0);
    DeleteObject(image->handle);
    image->handle = map;
}

void IT_FillColor(ImageBitmap* image, COLORREF color, RECT* region)
{
    colorRef_0 = color;
    HBITMAP map = CallIteratorOnPixels(image->handle, FillColorIterator, region, 0 , 0);
    DeleteObject(image->handle);
    image->handle = map;
    colorRef_0 = 0;
}

void IT_CopyToBuffer(ImageBitmap* image, PixelBuffer* buffer, RECT* region)
{
    if (buffer == 0) return;
    bufferRef_0 = buffer;
    HBITMAP map = CallIteratorOnPixels(image->handle, CopyToBufferIterator, region, 0, 0);
    DeleteObject(image->handle);
    image->handle = map;
    bufferRef_0 = 0;
}

void IT_PasteFromBuffer(ImageBitmap* image, PixelBuffer* buffer, RECT* region)
{
    if (buffer == 0) return;
    bufferRef_0 = buffer;
    HBITMAP map = CallIteratorOnPixels(image->handle, PasteFromBufferIterator, region, 0 , 0);
    DeleteObject(image->handle);
    image->handle = map;
    bufferRef_0 = 0;
}

void IT_PasteFromBufferColor(ImageBitmap* image, PixelBuffer* buffer, RECT* region)
{
    if (buffer == 0) return;
    bufferRef_0 = buffer;
    HBITMAP map = CallIteratorOnPixels(image->handle, PasteFromBufferColorIterator, region, 0, 0);
    DeleteObject(image->handle);
    image->handle = map;
    bufferRef_0 = 0;
}

void IT_InsertBitmap(ImageBitmap* image, HBITMAP bitmap, RECT* region)
{
    if (bitmap == 0) return;
    bufferRef_0 = NewPixelBuffer();

    HBITMAP result = CallIteratorOnPixels(bitmap, CopyToBufferIterator, 0,0,0);

    HBITMAP map = CallIteratorOnPixels(image->handle, InsertBitmapIterator, region,0 ,0);
    DeleteObject(image->handle);
    image->handle = map;
    DisposeObject(bufferRef_0);
    free(bufferRef_0);
    bufferRef_0 = 0;
    DeleteObject(result);
}

void IT_BlurRegion_N4(ImageBitmap* image, RECT* region)
{
    HBITMAP map = CallIteratorOnPixels(image->handle, N4_Blur_Iterator, region,0,0);
    DeleteObject(image->handle);
    image->handle = map;
}

void IT_Mirror(ImageBitmap* image, RECT* region, char isVertical_)
{
    isVertical = isVertical_;
    HBITMAP map = CallIteratorOnPixels(image->handle, MirrorIterator, region,0,0);
    DeleteObject(image->handle);
    image->handle = map;
}

void IT_AddBright(ImageBitmap* image, RECT* region, int bright)
{
    brightRef = bright;
    HBITMAP map = CallIteratorOnPixels(image->handle, BrightIterator, region,0,0);
    DeleteObject(image->handle);
    image->handle = map;
}

void IT_Task6(ImageBitmap* image)
{
    HBITMAP hBmp = image->handle;
    HBITMAP RetBmp = NULL;

    int newW = image->width * 3;
    int newH = image->height * 3;

    if (hBmp)
    {
        HDC BufferDC = CreateCompatibleDC(NULL);
        if (BufferDC)
        {
            HBITMAP hTmpBitmap = (HBITMAP)NULL;

            HGDIOBJ PreviousBufferObject = SelectObject(BufferDC, hBmp);

            HDC DirectDC = CreateCompatibleDC(NULL);
            if (DirectDC)
            {
                BITMAP bm;
                GetObject(hBmp, sizeof(bm), &bm);



                BITMAPINFO RGB32BitsBITMAPINFO;
                ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
                RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                RGB32BitsBITMAPINFO.bmiHeader.biWidth = newW;
                RGB32BitsBITMAPINFO.bmiHeader.biHeight = newH;
                RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
                RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;

                UINT* ptPixels;

                HBITMAP DirectBitmap = CreateDIBSection(DirectDC,
                    (BITMAPINFO*)&RGB32BitsBITMAPINFO,
                    DIB_RGB_COLORS,
                    (void**)&ptPixels,
                    NULL, 0);
                if (DirectBitmap)
                {
                    HGDIOBJ PreviousObject = SelectObject(DirectDC, DirectBitmap);
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            BitBlt(DirectDC, i * bm.bmWidth, j * bm.bmHeight,
                                bm.bmWidth, bm.bmHeight,
                                BufferDC, 0, 0, SRCCOPY);
                        }
                    }

                    SelectObject(DirectDC, PreviousObject);

                    RetBmp = DirectBitmap;
                }
                DeleteDC(DirectDC);
            }
            if (hTmpBitmap)
            {
                //SelectObject(hBmpDC, hBmp);
                DeleteObject(hTmpBitmap);
            }
            SelectObject(BufferDC, PreviousBufferObject);
            DeleteDC(BufferDC);
        }
    }
    if (image->handle != 0) {
        DeleteObject(image->handle);
    }
    image->handle = RetBmp;
    image->width = newW;
    image->height = newH;
}
