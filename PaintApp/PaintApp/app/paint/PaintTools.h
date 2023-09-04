#pragma once

#include <Windows.h>
#include "../core/Object.h"

#define PT_EXPORT __declspec(dllexport)


typedef struct tagPaintContext {

	int width;
	UINT color;
	HPEN pen;
	HBRUSH brush;
	char isFill;

	Object* canvasReference;

} PaintContext;

void InitPaintToolsResourses();

void UpdatePaintToolProperties(PaintContext* tool, int width, UINT color, char isFill);

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

PT_EXPORT typedef void (*PT_DrawFunc) (RECT* frame, PaintContext* context, HDC hdc, HWND hWnd);
PT_EXPORT typedef void (*PT_EventFunc) (RECT* region, PaintContext* context, HDC hdc, HWND hWnd);
//typedef void (*PaintToolDrawFunc) (RECT* rect, PaintToolProperties* props, HDC hdc, HWND hWnd);

/*

IPaintTool (extends Object):

@param onDraw - function to draw at frame (some rect in main canvas)
@param onDrawIcon - function, which calls on button icon render
@param onSessionCompleteEvent - function, which calls, when user release mouse button

*/
typedef struct tagIPaintTool {

	EXTENDS_OBJECT;

	void (*onDraw) (RECT* frame, PaintContext* context, HDC hdc, HWND hWnd);
	void (*onDrawIcon) (RECT* frame, PaintContext* context, HDC hdc, HWND hWnd);
	void (*onSessionCompleteEvent) (RECT* region, PaintContext* context, HDC hdc, HWND hWnd);

	const char* name;
		
} IPaintTool;

PT_EXPORT IPaintTool* CreateIPaintTool(PT_DrawFunc drawFunc, PT_DrawFunc drawIconFunc, const char* name);
PT_EXPORT IPaintTool* CreateIPaintToolEx(PT_DrawFunc drawFunc, PT_DrawFunc drawIconFunc, PT_EventFunc onSessionComplete, const char* name);

IPaintTool* CastToIPaintTool(Object* obj);

PT_EXPORT Object* GetPaintContextImageBitmap(PaintContext* context);

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */