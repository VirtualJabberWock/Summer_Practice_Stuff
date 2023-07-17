#pragma once
#include "PasteWindow.h"

DEFINE_WINDOWCLASS_TYPE(PasteWindow);

static PasteWindow* this = 0;

static HPEN borderPen;

static HWND OnCreate(PasteWindow* window, WindowContext* optParent) {
    if (optParent == 0) return 0;
    borderPen = CreatePen(PS_DOT, 1, 0x0055ff);
    HWND h = CreateWindowExA(
        0,
        window->__wndClass.winClassName, "",
        WS_CHILD,
        window->x, window->y, window->bufferRef->wSize, window->bufferRef->hSize,
        optParent->hWnd,
        0, optParent->hInst,
        0
    );
    return h;
}

#include "..\win\SimpleGraphics.h"

static LRESULT MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT* ref = &this->__wndClass.context.rect;
        RECT rect = {
            ref->left+1,
            ref->top+1,
            ref->right-1,
            ref->bottom-1,
        };
        

        HDC  hdcMem;
        HGDIOBJ oldBitmap;
        PRECT prect;

        hdcMem = CreateCompatibleDC(hdc);

        oldBitmap = SelectObject(hdcMem, this->bufferBitmap);

        BitBlt(ps.hdc,
            0, 0,
            this->bufferRef->wSize, this->bufferRef->hSize,
            hdcMem,
            0, 0,
            SRCCOPY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);

        DrawRect(hdc, &rect, borderPen);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_LBUTTONDOWN:
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);;
}

static LRESULT OnMessage(PasteWindow* window, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return MyWndProc(hWnd, message, wParam, lParam);
}

static ATOM RegisterWnd(PasteWindow* window, HINSTANCE hInstance) {

    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MyWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground = 0;
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = window->__wndClass.winClassName;
    wcex.hIconSm = 0;

    return RegisterClassExA(&wcex);
}

static void OnDispose(PasteWindow* window) {

    this = 0;

}

IWindowClass* NewPasteWindow(PixelBuffer* bufferRef)
{

    if (this != 0) {
        return this;
    }

    PasteWindow* pasteWindow =
        calloc(1, sizeof(PasteWindow));
    if (pasteWindow == 0) {
        return debugMemError();
    }
    WINDOWCLASS_SUPER(PasteWindow, pasteWindow);
    OverrideWindowClassRegister(PasteWindow, RegisterWnd);
    OverrideWindowClassOnMessage(PasteWindow, OnMessage);
    OverrideWindowClassCreate(PasteWindow, OnCreate);
    OverrideObjectDispose(PasteWindow, OnDispose);
    pasteWindow->bufferRef = bufferRef;
    this = pasteWindow;
    return pasteWindow;
}

void PasteWindowUpdate(PasteWindow* pasteWindow, PixelBuffer* bufferRef, int x, int y)
{

    HDC hdc = CreateCompatibleDC(0);

    char* buffer = NULL;

    __int64 fullSize = pasteWindow->bufferRef->wSize * pasteWindow->bufferRef->hSize * 3;

    BITMAPINFO bm = { 
        sizeof(BITMAPINFOHEADER),
        pasteWindow->bufferRef->wSize,
        pasteWindow->bufferRef->hSize,
        1, 24,
       BI_RGB,
       fullSize,
       0, 0, 0, 0 
    };
    pasteWindow->bufferBitmap = CreateDIBSection(hdc, &bm, DIB_RGB_COLORS, (void**)&buffer, 0, 0);



    memset(buffer, 255,fullSize);

    DeleteDC(hdc);

    pasteWindow->x = x;
    pasteWindow->y = y;
    pasteWindow->bufferRef = bufferRef;
}
