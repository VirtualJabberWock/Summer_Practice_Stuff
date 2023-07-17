#pragma once

#include <Windows.h>


typedef struct PaintTool {

	int width;
	UINT color;
	HPEN pen;
	HBRUSH brush;
	char isFill;

} PaintTool;

void InitPaintToolsResourses();

void UpdatePaintTool(PaintTool* tool, int width, UINT color, char isFill);


void RenderSquareTool(RECT* rect, PaintTool* tool, HDC hdc, HWND hWnd);
void RenderLineTool(RECT* rect, PaintTool* tool, HDC hdc, HWND hWnd);