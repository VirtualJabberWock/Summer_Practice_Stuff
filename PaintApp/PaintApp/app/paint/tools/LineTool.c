#include "LineTool.h"

#include "../../win/SimpleGraphics.h"

static HPEN toolIconRectEdge = 0;

void RenderLineTool(RECT* frame, PaintContext* props, HDC hdc, HWND hWnd)
{
	DrawLine(
		hdc,
		frame->left, frame->top, frame->right, frame->bottom,
		props->pen
	);
}

static void RenderIcon(RECT* rect, PaintContext* tool, HDC hdc, HWND hWnd) 
{
	DrawLine(
		hdc,
		rect->left + 8, rect->top + 5, rect->right - 8, rect->bottom - 5, 
		toolIconRectEdge
	);
}

void OnComplete(RECT* rect, PaintContext* tool, HDC hdc, HWND hWnd)
{
	int* xSide = (rect->right > rect->left) ? &rect->right : &rect->left;
	int* xSide0 = (rect->right > rect->left) ? &rect->left: &rect->right;
	int* ySide = (rect->bottom > rect->top) ? &rect->bottom : &rect->top;
	int* ySide0 = (rect->bottom > rect->top) ? &rect->top : &rect->bottom;

	*xSide = *xSide + tool->width;
	*xSide0 = *xSide0 - tool->width;
	*ySide = *ySide + tool->width;
	*ySide0 = *ySide0 - tool->width;
}

IPaintTool* CreateLineTool()
{
	toolIconRectEdge = CreatePen(PS_SOLID, 2, 0);
	return CreateIPaintToolEx(RenderLineTool, RenderIcon, doNothing, OnComplete, "LineTool");
}
