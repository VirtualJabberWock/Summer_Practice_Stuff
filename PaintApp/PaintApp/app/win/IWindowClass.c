#include "IWindowClass.h"
#pragma once


DEFINE_OBJECT_TYPE_ONLY(IWindowClass);

#include "..\core\String.h"
#include "..\core\CommonTypes.h"
#include "..\core\DebugUtil.h"

#include "..\win\Dialog.h"

#include <string.h>

void IWindowRegister(IWindowClass* window, HINSTANCE hInstance)
{
	if (window->isRegistred) return;
	int status = window->methods->registerClass(window, hInstance);
	window->isRegistred = (status != 0);
}

BOOL IWindowCreateAndShow(IWindowClass* window, WindowContext* optParent)
{
	if (!window->isRegistred) {
		debugShowWarning("Try to create not registred window!");
		return;
	}
	HWND h = window->methods->create(window, optParent);

	if (h == 0) {
		return debugFatalErrorFormat(
			"Window %s return 0, when called create function!",
			window->winClassName
		);
	}
	InitWindowContext(&window->context, h, optParent, optParent->hInst);
	ShowWindow(h, 1);
	UpdateWindow(h);
	return 1;
}


LRESULT CALLBACK IWindowSendMessage(IWindowClass* window, UINT message, WPARAM wParam, LPARAM lParam)
{
	return window->methods->OnMessage(window, window->context.hWnd, message, wParam, lParam);
}

void IWindowInvalidate(IWindowClass* window, RECT* optionalRegion)
{
	if (window->context.hWnd == 0) return;
	InvalidateRect(window->context.hWnd, optionalRegion, 0);
}

int IWindowClose(IWindowClass* window)
{
	return window->methods->OnMessage(window, window->context.hWnd, WM_CLOSE, 0, 0);
}

void InitWindowClass(IWindowClass* map, char* className, objectInternalMethod* methods, objectInternalMethod* objectMethods)
{
	InitObject(map, TYPE_IWindowClass, objectMethods);
	int len = strlen(className);
	map->winClassName = calloc((size_t)len + 1, sizeof(char));
	if (map->winClassName == 0) {
		debugMemError();
		return;
	}
	for (int i = 0; i < len; i++) {
		map->winClassName[i] = className[i];
	}
	map->winClassName[len] = 0;
	map->methods = methods;
}

HINSTANCE IWindowGetHINSTANCE(IWindowClass* window)
{
	return window->context.hInst;
}

LRESULT IWindowClass_defaultMessageHandler(IWindowClass* window, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
