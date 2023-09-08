#pragma once

#define GET_RED(color) (color & 0xff)
#define GET_GREEN(color) ((color & 0x00ff00) >> 8)
#define GET_BLUE(color) ((color & 0xff0000) >> 16)

#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) \
                                 | ((Color << 16) & 0xff0000)

#define MAKE_COL(r,g,b) ((r) + ((g) << 8) + ((b) << 16))

#include "ImageLoader.h"


typedef void (*PixelIterator) (UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo);

HBITMAP CallIteratorOnPixels(HBITMAP hBmp, PixelIterator iterator, RECT* rect, int newW, int newH);

//HBITMAP ReplaceColor(HBITMAP hBmp, COLORREF cOldColor, COLORREF cNewColor, HDC hBmpDC);

void IT_ReplaceColor(ImageBitmap* image, COLORREF cOldColor, COLORREF cNewColor);
void IT_InvertColor(ImageBitmap* image, RECT* region);
void IT_BlackAndWhiteColor(ImageBitmap* image, RECT* region);
void IT_FillColor(ImageBitmap* image, COLORREF color, RECT* region);

void IT_CopyToBuffer(ImageBitmap* image, PixelBuffer* buffer, RECT* region);
void IT_PasteFromBuffer(ImageBitmap* image, PixelBuffer* buffer, RECT* region);
void IT_PasteFromBufferColor(ImageBitmap* image, PixelBuffer* buffer, RECT* region);

void IT_InsertBitmap(ImageBitmap* image, HBITMAP bitmap, RECT* region);

void IT_BlurRegion_N4(ImageBitmap* image, RECT* region);
void IT_Mirror(ImageBitmap* image, RECT* region, char isVertical);
void IT_AddBright(ImageBitmap* image, RECT* region, int bright);
void IT_Task6(ImageBitmap* image);

__declspec(dllexport) void TransformImageBuffer(ImageBitmap* image, PixelIterator transformer);