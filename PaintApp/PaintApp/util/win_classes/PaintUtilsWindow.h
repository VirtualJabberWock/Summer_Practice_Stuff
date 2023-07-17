#pragma once
#include "..\win\IWindowClass.h"
#include "MainFrameChunks.h"
#include "CanvasWindow.h"
#include "..\win\ButtonUI.h"
#include "..\container\ObjectVector.h"

typedef struct PaintUtilsWindow {

	EXTENDS_WINDOWCLASS;

	CanvasWindow* canvasWindow;
	ObjectVector* buttons;

} PaintUtilsWindow;

IWindowClass* NewPaintUtilsWindow(CanvasWindow* canvasWindow);