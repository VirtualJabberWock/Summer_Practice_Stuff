#include "CanvasStatusWindow.h"

DEFINE_WINDOWCLASS_TYPE(CanvasStatusWindow);

static CanvasStatusWindow* canvasInstance;

char textBuffer[200] = { 0 };

static HWND OnCreate(CanvasStatusWindow* window, WindowContext* optParent) {
    if (optParent == 0) return 0;
    HWND h = CreateWindowExA(
        0,
        window->__wndClass.winClassName, "",
        WS_CHILD | WS_BORDER,
        0, optParent->height - STATUS_HEIGHT, optParent->width, STATUS_HEIGHT,
        optParent->hWnd,
        0, optParent->hInst,
        0
    );

    return h;
}

#include <stdio.h>



static void OnDraw(HWND hWnd, HDC hdc) {
    if (canvasInstance == 0) return;
    POINT p;
    RECT* ref = &canvasInstance->__wndClass.context.rect;
    RECT textR = { ref->left+16, ref->top+5, ref->right, ref->bottom };
    GetCursorPos(&p);
    ScreenToClient(canvasInstance->__wndClass.context.parent->hWnd, &p);
    snprintf(textBuffer, 200, "x: %d, y: %d",p.x, p.y - UTILS_HEIGHT);
    DrawTextA(hdc, textBuffer, 200, &textR, DT_NOCLIP);
}

static LRESULT MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        OnDraw(hWnd,hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);;
}

static LRESULT OnMessage(CanvasStatusWindow* window, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return MyWndProc(hWnd, message, wParam, lParam);
}

static ATOM RegisterWnd(CanvasStatusWindow* window, HINSTANCE hInstance) {

    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MyWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = window->__wndClass.winClassName;
    wcex.hIconSm = 0;

    return RegisterClassExA(&wcex);
}

static void OnDispose(CanvasStatusWindow* window) {
    canvasInstance = 0;
}

IWindowClass* NewCanvasStatusWindow()
{
    if (canvasInstance != 0) {
        return canvasInstance;
    }
	CanvasStatusWindow* canvasWindow =
		calloc(1, sizeof(CanvasStatusWindow));
	if (canvasWindow == 0) {
		return debugMemError();
	}
	WINDOWCLASS_SUPER(CanvasStatusWindow, canvasWindow);
    OverrideWindowClassRegister(CanvasStatusWindow, RegisterWnd);
    OverrideWindowClassOnMessage(CanvasStatusWindow, OnMessage);
    OverrideWindowClassCreate(CanvasStatusWindow, OnCreate);
    OverrideObjectDispose(CanvasStatusWindow, OnDispose);
    canvasInstance = canvasWindow;
	return canvasWindow;
}
