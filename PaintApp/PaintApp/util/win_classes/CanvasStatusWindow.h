#pragma once
#include "..\win\IWindowClass.h"
#include "MainFrameChunks.h"

typedef struct CanvasStatusWindow {

	EXTENDS_WINDOWCLASS;



} CanvasStatusWindow;

IWindowClass* NewCanvasStatusWindow();
