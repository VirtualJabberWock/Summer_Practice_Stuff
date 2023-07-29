#pragma once
#include "..\win\IWindowClass.h"
#include "MainFrameChunks.h"
#include "CanvasStatusWindow.h"
#include "..\image\ImageLoader.h"

typedef struct PasteWindow {

	EXTENDS_WINDOWCLASS;

	int x;
	int y;
	PixelBuffer* bufferRef;
	HBITMAP bufferBitmap;

} PasteWindow;

IWindowClass* NewPasteWindow(PixelBuffer* bufferRef);

void PasteWindowUpdate(PasteWindow* pasteWindow, PixelBuffer* bufferRef, int x, int y);