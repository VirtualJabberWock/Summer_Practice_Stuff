#pragma once

#include "..\core\Object.h"
#include <Windows.h>
#include "..\win\IWindowClass.h"

#include "GDIPlusBridge.h"

typedef enum enumImageFileFormat {

	IMAGE_FORMAT_BMP,
	IMAGE_FORMAT_JPG,
	IMAGE_FORMAT_PNG

} ImageFileFormat;

typedef struct tagImageBitmap{

	EXTENDS_OBJECT;

	HBITMAP handle;
	int width;
	int height;
	ImageFileFormat format;

} ImageBitmap;

typedef struct tagPixelBuffer {

	EXTENDS_OBJECT;
	int used;
	UINT* pixels;
	int wSize;
	int hSize;

} PixelBuffer;

ImageBitmap* ImageLoader_LoadImage(LPCWSTR filename, IWindowClass* window);

void ImageLoader_Save(ImageBitmap* image, LPCWSTR filename, IWindowClass* window);

void IL_Utils_SaveBitmap(HBITMAP* bmp, LPCWSTR filename);

int isPointOnImage(ImageBitmap* image, int x, int y);


PixelBuffer* NewPixelBuffer();
int CopyPixelBuffer(PixelBuffer* destination, PixelBuffer* from);
void ClearAndResizePixelBuffer(PixelBuffer* buffer, int width, int height);

