#pragma once
#include "..\win\IWindowClass.h"
#include "CanvasWindow.h"
#include "..\container\ObjectVector.h"
#include "../paint/PaintTools.h"

typedef struct PaintUtilsWindow {

	EXTENDS_WINDOWCLASS;

	CanvasWindow* canvasWindow;
	ObjectVector* buttons;
	ObjectVector* toolButtons;

	PaintContext* paintProps;

} PaintUtilsWindow;

IWindowClass* NewPaintUtilsWindow(CanvasWindow* canvasWindow);