#pragma once
#include "..\win\IWindowClass.h"
#include "MainFrameChunks.h"
#include "CanvasStatusWindow.h"
#include "..\image\ImageLoader.h"
#include "..\win\PaintTools.h"
#include "PasteWindow.h"
#include "../core/messaging/State.h"

typedef enum enumPaintUtils {

	PAINT_COLOR_TOOL = -2,

	PAINT_NO_TOOL = -1,

	PAINT_SELECT_TOOL = 0,
	PAINT_SQUARE_TOOL = 1,
	PAINT_LINE_TOOL = 2,
	PAINT_BRUSH_TOOL = 3

} PaintUtilType;

typedef struct CanvasWindow {

	EXTENDS_WINDOWCLASS;

	CanvasStatusWindow* statusRef;
	ImageBitmap* mainImage;
	PixelBuffer* imageBuffer;

	PaintUtilType toolType;
	PaintTool* paintTool;

	PasteWindow* pasteWindow;

	State* mouseControlState;

} CanvasWindow;


IWindowClass* GetCanvasWindow(CanvasStatusWindow* statusRef);

void CanvasOnSaveImage(CanvasWindow* canvasWindow);
