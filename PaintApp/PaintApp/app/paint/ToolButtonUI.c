#include "ToolButtonUI.h"

#include "PaintToolsLoader.h"

ToolButton* NewToolButton(
	int x, int y, int width, int height,
	OnButtonClick action, ButtonDrawProc drawFunction, const char* toolName
)
{
	ToolButton* button = calloc(1, sizeof(ToolButton));
	if (button == 0) {
		return debugMemError();
	}

	ButtonInit(button, x, y, width, height, action, drawFunction);

	button->btn.tag = 0;
	
	button->toolReference = GetLoadedPaintTool(toolName);
	if (button->toolReference == 0) {
		return debugFatalErrorFormat("ToolButton: Error paint tool <%s> don't loaded!", toolName);
	}
	return button;
}

void UI_ToolButton_BasicDraw(ToolButton* btn, PaintContext* props, HDC hdc, HWND hWnd)
{
	btn->toolReference->onDrawIcon(&btn->btn.rect, props, hdc, hWnd, btn->btn.state);
}
