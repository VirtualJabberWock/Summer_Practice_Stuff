#pragma once

#include "..\core\Object.h"
#include <Windows.h>

#include "..\core\Object.h"

typedef int (*OnButtonUIClick) (Object* button);
typedef int (*ButtonUIDrawProc) (Object* button, HDC hdc, HWND hWnd);

typedef enum enumButtonState {
	BUTTON_STATE_UNACTIVE = -1,
	BUTTON_STATE_ACTIVE = 0,
	BUTTON_STATE_FOCUS = 1,
	BUTTON_STATE_CLICK = 2,
	BUTTON_STATE_SELECTED = 3
} ButtonUIState;

typedef struct tagButtonUI {

	EXTENDS_OBJECT;

	RECT rect;
	OnButtonUIClick action;
	ButtonUIState state;
	ButtonUIDrawProc drawFunction;
	char rectValidation;
	int tag;

} ButtonUI;

ButtonUI* NewButtonUI(int x, int y, int width, int height, OnButtonUIClick action, ButtonUIDrawProc drawFunction, int tag);

void UI_ButtonNotify(ButtonUI* btn, int mx, int my, int isClick, HWND hWnd);
void UI_ButtonDraw(ButtonUI* btn, HDC hdc, HWND hWnd);

void UI_Button_BasicDraw(ButtonUI* btn, HDC hdc, HWND hWnd);
