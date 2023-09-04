#pragma once

#include "../win/ButtonUI.h"
#include "PaintTools.h"

typedef struct tagToolButtonUI {

	IMPLEMENT_BUTTONUI;

	IPaintTool* toolReference;

} ToolButton;

ToolButton* NewToolButton(
	int x, int y, int width, int height,
	OnButtonClick action, ButtonDrawProc drawFunction, const char* toolName
);

void UI_ToolButton_BasicDraw(ToolButton* btn, PaintContext* props, HDC hdc, HWND hWnd);