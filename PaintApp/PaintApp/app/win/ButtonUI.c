#include "ButtonUI.h"

DEFINE_OBJ_TYPE(Button);

Button* NewButton(int x, int y, int width, int height, OnButtonClick action, ButtonDrawProc drawFunction, int tag)
{
	Button* button = calloc(1, sizeof(Button));
	if (button == 0) {
		return debugMemError();
	}

	ButtonInit(button, x, y, width, height, action, drawFunction);

	button->tag = tag;

	return button;
}

void ButtonInit(Button* button, int x, int y, int width, int height, OnButtonClick action, ButtonDrawProc drawFunction)
{
	OBJECT_SUPER(Button, button);

	button->action = action;
	button->drawFunction = drawFunction;
	button->state = BUTTON_STATE_ACTIVE;

	button->rect.left = x;
	button->rect.top = y;
	button->rect.right = x + width;
	button->rect.bottom = y + height;
	button->rectValidation = 0;
}


void UI_ButtonNotify(Button* btn, int mx, int my, int isClick, HWND hWnd)
{
	if (btn->__obj.type != TYPE_Button) {
		return;
	}

	if (mx >= btn->rect.left
		&& mx <= btn->rect.right
		&& my >= btn->rect.top &&
		my <= btn->rect.bottom) {


		if (isClick == 1) {
			btn->action(btn);
			RECT vRect = {
					btn->rect.left - 1,
					btn->rect.top - 1,
					btn->rect.right + 1,
					btn->rect.bottom + 1,
			};
			InvalidateRect(hWnd, &vRect, 0);
		}
		else if (btn->state != BUTTON_STATE_SELECTED) {
			if (isClick == 0) {
				btn->state = BUTTON_STATE_FOCUS;
			}
			if (btn->rectValidation == 0) {
				RECT vRect = {
					btn->rect.left - 1,
					btn->rect.top - 1,
					btn->rect.right + 1,
					btn->rect.bottom + 1,
				};
				InvalidateRect(hWnd, &vRect, 0);
				btn->rectValidation = 1;
			}
		}
	}
	else if (btn->state != BUTTON_STATE_SELECTED) {
		if (btn->rectValidation == 1) {
			RECT vRect = {
			btn->rect.left - 1,
			btn->rect.top - 1,
			btn->rect.right + 1,
			btn->rect.bottom + 1,
			};
			InvalidateRect(hWnd, &vRect, 0);
			btn->rectValidation = 0;
		}
		btn->state = BUTTON_STATE_ACTIVE;
	}

}

void UI_ButtonDraw(Button* btn, HDC hdc, HWND hWnd)
{
	btn->drawFunction(btn, hdc, hWnd);
}

#include "SimpleGraphics.h"
#include "../appearance/Theme.h"

void UI_Button_BasicDraw(Button* btn, HDC hdc, HWND hWnd)
{

	if (btn->state == BUTTON_STATE_SELECTED) {
		FillRect(hdc, &btn->rect, Theme_B_buttonPressed);
		DrawRect(hdc, &btn->rect, Theme_P_buttonPressed);
		return;
	}

	if (btn->state == BUTTON_STATE_FOCUS) {
		FillRect(hdc, &btn->rect, Theme_B_buttonFocus);
		DrawRect(hdc, &btn->rect, Theme_P_buttonFocus);
	}
	else {
		FillRect(hdc, &btn->rect, Theme_B_button);
		DrawRect(hdc, &btn->rect, Theme_P_button);
	}
	
}
