#pragma once

#include <Windows.h>
#include "..\core\Object.h"

typedef int (*OnButtonClick) (Object* button);
typedef int (*ButtonDrawProc) (Object* button, HDC hdc, HWND hWnd);

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
	OnButtonClick action;
	ButtonUIState state;
	ButtonDrawProc drawFunction;
	char rectValidation;
	int tag;

} Button;

#define IMPLEMENT_BUTTONUI Button btn

Button* NewButton(int x, int y, int width, int height, OnButtonClick action, ButtonDrawProc drawFunction, int tag);
void ButtonInit(Button* button, int x, int y, int width, int height, OnButtonClick action, ButtonDrawProc drawFunction);

void UI_ButtonNotify(Button* btn, int mx, int my, int isClick, HWND hWnd);
void UI_ButtonDraw(Button* btn, HDC hdc, HWND hWnd);

void UI_Button_BasicDraw(Button* btn, HDC hdc, HWND hWnd);
