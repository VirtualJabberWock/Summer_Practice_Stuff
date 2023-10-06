#pragma once
#include "..\win\IWindowClass.h"
#include "MainFrameChunks.h"
#include "CanvasStatusWindow.h"
#include "..\image\ImageLoader.h"
#include "..\paint\PaintTools.h"
#include "PasteWindow.h"
#include "../core/messaging/State.h"
#include "../container/map/HashMap.h"

typedef enum enumPaintUtils {

	PAINT_NO_TOOL = -1,

	PAINT_SELECT_TOOL = 0,
	PAINT_DRAW_TOOL = 1

} PaintUtilType;

typedef struct CanvasWindow {

	EXTENDS_WINDOWCLASS;

	CanvasStatusWindow* statusRef;
	ImageBitmap* mainImage;
	PixelBuffer* pasteBuffer;

	PaintUtilType toolType;
	PaintContext* paintToolProperties;
	IPaintTool* paintTool;

	PasteWindow* pasteWindow;

	BOOL isCanvasInvalidated;

	State* mouseControlState;

} CanvasWindow;


IWindowClass* GetCanvasWindow(CanvasStatusWindow* statusRef);

void CanvasOnSaveImage(CanvasWindow* canvasWindow);
