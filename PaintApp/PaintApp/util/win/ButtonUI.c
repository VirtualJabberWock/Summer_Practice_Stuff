#include "ButtonUI.h"

DEFINE_OBJ_TYPE(ButtonUI);

HBRUSH testBrushActive = 0;
HBRUSH testBrushFocus = 0;
HBRUSH testBrushSelect = 0;
HPEN testPenActive = 0;
HPEN testPenFocus = 0;
HPEN testPenSelect = 0;

static int basicResoursesInitialized = 0;

static void InitializeBasicResources() {

	if (basicResoursesInitialized) return;

	testBrushActive = CreateSolidBrush(0x00c0c0c0);
	testBrushFocus = CreateSolidBrush(0x00eddaac);
	testBrushSelect = CreateSolidBrush(0x00ef9d5b);
	testPenActive = CreatePen(PS_SOLID, 1, 0x999999);
	testPenFocus = CreatePen(PS_SOLID, 1, 0xd9c492);
	testPenSelect = CreatePen(PS_SOLID, 1, 0xdc7e31);

	basicResoursesInitialized = 1;
}

ButtonUI* NewButtonUI(int x, int y, int width, int height, OnButtonUIClick action, ButtonUIDrawProc drawFunction, int tag)
{
	ButtonUI* button = calloc(1, sizeof(ButtonUI));
	if (button == 0) {
		return debugMemError();
	}

	OBJECT_SUPER(ButtonUI, button);

	button->action = action;
	button->drawFunction = drawFunction;
	button->state = BUTTON_STATE_ACTIVE;

	button->rect.left = x;
	button->rect.top= y;
	button->rect.right = x + width;
	button->rect.bottom = y + height;
	button->rectValidation = 0;

	button->tag = tag;

	InitializeBasicResources();

	return button;
}


void UI_ButtonNotify(ButtonUI* btn, int mx, int my, int isClick, HWND hWnd)
{
	if (btn->__obj.type != ButtonUI_TYPE) {
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

void UI_ButtonDraw(ButtonUI* btn, HDC hdc, HWND hWnd)
{
	btn->drawFunction(btn, hdc, hWnd);
}

#include "SimpleGraphics.h"

void UI_Button_BasicDraw(ButtonUI* btn, HDC hdc, HWND hWnd)
{

	if (btn->state == BUTTON_STATE_SELECTED) {
		FillRect(hdc, &btn->rect, testBrushSelect);
		DrawRect(hdc, &btn->rect, testPenSelect);
		return;
	}

	if (btn->state == BUTTON_STATE_FOCUS) {
		FillRect(hdc, &btn->rect, testBrushFocus);
		DrawRect(hdc, &btn->rect, testPenFocus);
	}
	else {
		FillRect(hdc, &btn->rect, testBrushActive);
		DrawRect(hdc, &btn->rect, testPenActive);
	}
	
}
