#pragma once
#include <Windows.h>

typedef struct tagWindowContext{

	HWND hWnd;
	struct tagWindowContext* parent;
	HINSTANCE hInst;
	size_t width;
	size_t height;
	RECT rect;

} WindowContext;

void InitWindowContext(WindowContext* context, HWND hWnd, WindowContext* parent, HINSTANCE hInst);