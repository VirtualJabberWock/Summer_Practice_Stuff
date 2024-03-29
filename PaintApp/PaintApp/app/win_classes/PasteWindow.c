#pragma once
#include "PasteWindow.h"
#include <stdio.h>

DEFINE_WINDOWCLASS_TYPE(PasteWindow);

static PasteWindow* this = 0;

static HPEN borderPen;

static char dragWindow = 0;
static int g_lastMouseX = 0;
static int g_lastMouseY = 0;

static HWND OnCreate(PasteWindow* window, WindowContext* optParent) {
    if (optParent == 0) return 0;
    borderPen = CreatePen(PS_DOT, 1, 0x0055ff);
    HWND h = CreateWindowExA(
        0,
        window->__wndClass.winClassName, "",
        WS_CHILD | WS_THICKFRAME,
        window->x, window->y, window->bufferRef->wSize, window->bufferRef->hSize,
        optParent->hWnd,
        0, optParent->hInst,
        0
    );
    return h;
}

#include "..\win\SimpleGraphics.h"
#include "../image/ImageScaleUtil.h"

static LRESULT MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_ERASEBKGND:
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT ref;
        GetClientRect(hWnd, &ref);
        RECT rect = {
            ref.left,
            ref.top,
            ref.right-1,
            ref.bottom-1,
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

        if (dragWindow == 0) {
            DrawRect(hdc, &rect, borderPen);
        }
        EndPaint(hWnd, &ps);
    }
    break;
   
    case WM_NCCALCSIZE:
        return 0;

    case WM_NCHITTEST:
    {
        char bLeft = 0, bRight = 0, bTop = 0, bBottom = 0;
        char bHorz = 0, bVert = 0;
        RECT rWnd;

        GetWindowRect(hWnd, &rWnd);

        if (LOWORD(lParam) > rWnd.left && LOWORD(lParam) < rWnd.left + 4)      bLeft = 1;
        if (LOWORD(lParam) > rWnd.right - 4 && LOWORD(lParam) < rWnd.right)    bRight = 1;
        if (HIWORD(lParam) > rWnd.top && HIWORD(lParam) < rWnd.top + 4)        bTop = 1;
        if (HIWORD(lParam) > rWnd.bottom - 4 && HIWORD(lParam) < rWnd.bottom)  bBottom = 1;

        if (bLeft && bTop)      return HTTOPLEFT;
        if (bRight && bTop)     return HTTOPRIGHT;
        if (bLeft && bBottom)   return HTBOTTOMLEFT;
        if (bRight && bBottom)  return HTBOTTOMRIGHT;

        if (bLeft)    return HTLEFT;
        if (bRight)   return HTRIGHT;
        if (bTop)     return HTTOP;
        if (bBottom)  return HTBOTTOM;

        if (bHorz && bVert)  return HTCAPTION;
        this->isBitmapRenderLocked = 1;
        int nchResult = DefWindowProc(hWnd, WM_NCHITTEST, wParam, lParam);
        return nchResult;
    }

    case WM_CAPTURECHANGED:
        if (this == 0) return debugFatalError("PasteWindow::this is null");
        RECT rect;
        GetWindowRect(hWnd, &rect);
        POINT pos;
        pos.x = rect.left;
        pos.y = rect.top;
        ScreenToClient(this->__wndClass.context.parent->hWnd, &pos);
        float changeX = (float)(rect.right - rect.left) / (float)(this->bufferRef->wSize);
        float changeY = (float)(rect.bottom - rect.top) / (float)(this->bufferRef->hSize);
        ImageScale_SimpleRect(this->bufferRef, 0, changeX, changeY);
        PasteWindowUpdate(this, this->bufferRef, pos.x, pos.y);
        this->__wndClass.context.width = this->bufferRef->wSize;
        this->__wndClass.context.height = this->bufferRef->hSize;
        this->isBitmapRenderLocked = 0;
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_INTERNALPAINT);
        return DefWindowProc(hWnd, message, wParam, lParam);
    case WM_LBUTTONDOWN:
        dragWindow = 1;
        g_lastMouseX = LOWORD(lParam);
        g_lastMouseY = HIWORD(lParam);
        SetCapture(hWnd);
        InvalidateRect(hWnd, 0, 0);
        break;
    case WM_LBUTTONUP:
        ReleaseCapture();
        dragWindow = 0;
        InvalidateRect(hWnd, 0, 0);
        break;
    case WM_MOUSEMOVE:
        if (dragWindow == 1)
        {
            POINT pos;
            //POINT pos2;

            pos.x = (int)(short)LOWORD(lParam);
            pos.y = (int)(short)HIWORD(lParam);

            this->x += pos.x - g_lastMouseX;
            this->y += pos.y - g_lastMouseY;

            MoveWindow(hWnd, this->x, this->y, this->__wndClass.context.width, this->__wndClass.context.height, TRUE);
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
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
    wcex.hCursor = LoadCursor(0, IDC_SIZEALL);
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

#include "../image/ImageTransform.h"

void PasteWindowUpdate(PasteWindow* pasteWindow, PixelBuffer* bufferRef, int x, int y)
{

    HDC hdc = CreateCompatibleDC(0);

    char* buffer = NULL;

    pasteWindow->bufferRef = bufferRef;

    __int64 fullSize = pasteWindow->bufferRef->wSize * pasteWindow->bufferRef->hSize;

    BITMAPINFO bm = { 
        sizeof(BITMAPINFOHEADER),
        pasteWindow->bufferRef->wSize,
        pasteWindow->bufferRef->hSize,
        1, 32,
       BI_RGB,
       fullSize*4,
       0, 0, 0, 0 
    };
    pasteWindow->bufferBitmap = CreateDIBSection(hdc, &bm, DIB_RGB_COLORS, (void**)&buffer, 0, 0);

    for (int i = 0; i < fullSize; i++) {
        buffer[i*4]   = GET_RED(bufferRef->pixels[i]);
        buffer[i*4+1] = GET_GREEN(bufferRef->pixels[i]);
        buffer[i*4+2] = GET_BLUE(bufferRef->pixels[i]);
    }

    DeleteDC(hdc);

    pasteWindow->x = x;
    pasteWindow->y = y;
    pasteWindow->isBitmapRenderLocked = 0;
    

    InvalidateRect(pasteWindow->__wndClass.context.hWnd, 0, 0);
}
