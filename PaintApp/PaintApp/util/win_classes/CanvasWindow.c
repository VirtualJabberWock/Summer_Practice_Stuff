#include "CanvasWindow.h"

#include "..\win\SimpleGraphics.h"
#include "..\win\RegionSelection.h"
#include "..\win\Dialog.h"
#include "..\image\ImageLoader.h"
#include "..\win\PaintTools.h"

DEFINE_WINDOWCLASS_TYPE(CanvasWindow);

static CanvasWindow* this;
static HBRUSH backGroundBrush;
static Selection selection;

static ImageBitmap* mainImage;

static BOOL fBlt;            // TRUE if BitBlt occurred 
static BOOL fScroll;         // TRUE if scrolling occurred 
static BOOL fSize;

static int xMinScroll;    
//current scroll x value
static int xS;   
static int xMaxScroll;       
 
static int yMinScroll;     
//current scroll y value
static int yS;   
static int yMaxScroll;      

static BOOL isCanvasNew = 1;

#define ID_COPY 101
#define ID_CUT 102
#define ID_PASTE 103
#define ID_INVERT 104
#define ID_BW 105
#define ID_PASTECOLOR 106
#define ID_TESTN4 107
#define ID_MIRROR_V 108
#define ID_MIRROR_H 109
#define ID_ADD_BRIGHT 110
#define ID_SUB_BRIGHT 111

POINT lastMousePos;

static HWND OnCreate(CanvasWindow* window, WindowContext* optParent) {
    if (optParent == 0) return 0;
    InitializeSelectionResources();

    window->mainImage = ImageLoader_LoadBitmap(L"test7.bmp", window);

    HWND h = CreateWindowExA(
        0,
        window->__wndClass.winClassName, "",
        WS_CHILD | WS_HSCROLL | WS_VSCROLL,
        0, UTILS_HEIGHT, optParent->width, optParent->height - UTILS_HEIGHT - STATUS_HEIGHT,
        optParent->hWnd, 0,
        optParent->hInst,
        0
    );

    fSize = 1;

    return h;
}

#include "..\image\ImageTransform.h"
#include "../../resource.h"

static void OnMouseMenuClick(int id, int mx_offset, int my_offset) {

    RECT mouseRegion = { mx_offset, my_offset, mx_offset, my_offset };

    switch (id)
    {
    case ID_CUT:
        IT_CopyToBuffer(this->mainImage, this->imageBuffer, &selection.rect);
        IT_FillColor(this->mainImage, this->paintTool->color, &selection.rect);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_COPY:
        IT_CopyToBuffer(this->mainImage, this->imageBuffer, &selection.rect);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_PASTE:
        IT_PasteFromBuffer(this->mainImage, this->imageBuffer, &mouseRegion);
        isCanvasNew = 1;
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_PASTECOLOR:
        IT_PasteFromBufferColor(this->mainImage, this->imageBuffer, &mouseRegion);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_INVERT:
        IT_InvertColor(this->mainImage, &selection.rect);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_BW:
        IT_BlackAndWhiteColor(this->mainImage, &selection.rect);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_TESTN4:
        IT_BlurRegion_N4(this->mainImage, &selection.rect);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_MIRROR_H:
        IT_Mirror(this->mainImage, &selection.rect, 0);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_MIRROR_V:
        IT_Mirror(this->mainImage, &selection.rect, 1);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_ADD_BRIGHT:
        IT_AddBright(this->mainImage, &selection.rect, 10);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;
    case ID_SUB_BRIGHT:
        IT_AddBright(this->mainImage, &selection.rect, -10);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        break;

    default:
        break;
    }
}

static void OnMouseMove(HWND hWnd, int x, int y, int isLeftDown){
    if (!isPointOnImage(this->mainImage, x + xS, y + yS))
        return;
    lastMousePos.x = x;
    lastMousePos.y = y;
    IWindowInvalidate(this->statusRef, 0);
    NotifySelection(&selection, x + xS, y + yS, hWnd);
}

static void OnPaint(HWND hWnd, HDC hdc, PAINTSTRUCT ps) {

    if (this == 0) return;

    RECT* ref = &this->__wndClass.context.rect;
    if (isCanvasNew) {
        FillRect(hdc, ref, backGroundBrush);
        isCanvasNew = 0;
    }

    HDC  hdcMem;
    HGDIOBJ oldBitmap;
    PRECT prect;

    hdcMem = CreateCompatibleDC(hdc);

    oldBitmap = SelectObject(hdcMem, this->mainImage->handle);

    BitBlt(ps.hdc,
            0, 0,
            this->mainImage->width, this->mainImage->height,
            hdcMem,
            xS, yS,
            SRCCOPY);

    
    if (fScroll)
    {
        FillRect(hdc, ref, backGroundBrush);
        prect = &ps.rcPaint;

        BitBlt(ps.hdc,
            prect->left, prect->top,
            (prect->right - prect->left),
            (prect->bottom - prect->top),
            hdcMem,
            prect->left + xS,
            prect->top + yS,
            SRCCOPY);

        fScroll = FALSE;
    }
    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);

    RECT selectToScreen = {
        selection.rect.left - xS,
        selection.rect.top - yS,
        selection.rect.right - xS,
        selection.rect.bottom - yS,
    };
    
    if (this->toolType == PAINT_SELECT_TOOL) {
        DrawSelection(hdc, &selection, xS, yS);
    }

    if (selection.state == 1) {

        switch (this->toolType) {

        case PAINT_SQUARE_TOOL:
            RenderSquareTool(&selectToScreen, this->paintTool, hdc, hWnd);
            break;
        case PAINT_LINE_TOOL:
            RenderLineTool(&selectToScreen, this->paintTool, hdc, hWnd);
            break;

        }
    }
    
}

static HDC tempHDC = 0;

static void OnSelectionComplete(HDC hdc, HWND hWnd) {

    

    if (this->toolType == PAINT_SELECT_TOOL) return;
    if (this->toolType == PAINT_NO_TOOL) return;

    selection.state = 0;

    Sleep(1);

    int startX = min(selection.rect.left - xS, selection.rect.right - xS);
    int startY = min(selection.rect.top - yS, selection.rect.bottom - yS);

    int width = abs(selection.rect.right - selection.rect.left);
    int height = abs(selection.rect.bottom - selection.rect.top);

    HDC hdcScreen = GetDC(hWnd);
    HBITMAP hbmC;

    if (width == 0 && height == 0) return;

    if ((hbmC = CreateCompatibleBitmap(hdcScreen, width, height)) != NULL)
    {
        HDC hdcC;
        if ((hdcC = CreateCompatibleDC(hdcScreen)) != NULL)
        {
            HBITMAP hbmOld = (HBITMAP)SelectObject(hdcC, hbmC);

            BitBlt(hdcC, 0, 0, width , height, hdcScreen, startX, startY, SRCCOPY);

            SelectObject(hdcC, hbmOld);
            DeleteDC(hdcC);
        }
    }

    ReleaseDC(hWnd, hdcScreen);

    RECT __sRect = {
        startX + xS,
        startY + yS,
        startX + xS + width,
        startY + yS + height,
    };

    IT_InsertBitmap(this->mainImage, hbmC, &__sRect);

    if (hbmC != 0) {
        DeleteObject(hbmC);
    }
}

static void OnScrollPositionUpdated() {
    if (this == 0) return;
    InvalidateSelection(&selection, this->__wndClass.context.hWnd);
}

static LRESULT MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (this == 0) {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    int mx = LOWORD(lParam);
    int my = HIWORD(lParam);
    SCROLLINFO si;

    switch (message)
    {
    case WM_RBUTTONDOWN:
        if (isPointInSelection(&selection, mx + xS, my + yS) && this->toolType == PAINT_SELECT_TOOL) {
            MouseMenu menu; InitMouseMenu(&menu);
            AddOptionToMouseMenu(&menu, L"Размыть (N4+S)", ID_TESTN4, 1);
            AddOptionToMouseMenu(&menu, L"Уменьшить яркость (-10)", ID_SUB_BRIGHT, 1);
            AddOptionToMouseMenu(&menu, L"Увеличить яркость (+10)", ID_ADD_BRIGHT, 1);
            AddOptionToMouseMenu(&menu, L"Отразить (По горизонтали)", ID_MIRROR_H, 1);
            AddOptionToMouseMenu(&menu, L"Отразить (По вертикали)", ID_MIRROR_V, 1);
            AddOptionToMouseMenu(&menu, L"Отеннки серого (16 цветов)", ID_BW, 1);
            AddOptionToMouseMenu(&menu, L"Обратить цвета", ID_INVERT, 1);
            AddOptionToMouseMenu(&menu, L"Вырезать", ID_CUT, 1);
            AddOptionToMouseMenu(&menu, L"Копировать", ID_COPY, 1);
            OnMouseMenuClick(
                ShowMouseMenu(&menu, hWnd, mx, my), mx + xS, my + yS
            );
            DestroyMouseMenu(&menu);
        }
        else {
            MouseMenu menu; InitMouseMenu(&menu);
            AddOptionToMouseMenu(&menu, L"Вставить (Экран)", ID_PASTECOLOR, (this->imageBuffer->used));
            AddOptionToMouseMenu(&menu, L"Вставить", ID_PASTE, (this->imageBuffer->used));
            OnMouseMenuClick(
                ShowMouseMenu(&menu, hWnd, mx, my), mx + xS, my + yS
            );
            DestroyMouseMenu(&menu);
        }
        break;
    case WM_MOUSEMOVE:
        OnMouseMove(hWnd, mx, my, (wParam == MK_LBUTTON));
        break;
    case WM_LBUTTONDOWN:
        if (!isPointOnImage(this->mainImage, mx+xS, my+yS))
            break;
        StartSelection(&selection, mx + xS, my + yS);
        break;
    case WM_LBUTTONUP:
        OnSelectionComplete(tempHDC, hWnd);
        CompleteSelection(&selection, mx + xS, my + yS);
        InvalidateRect(hWnd, 0, 0);
        break;
    case WM_SETCURSOR:
        if (isPointInSelection(&selection, lastMousePos.x + xS, lastMousePos.y + yS)) {
            SetCursor(RegSelect_GetCursor(REG_SELECT_CURSOR_IN));
            return 0;
        }
        else {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        tempHDC = hdc;
        OnPaint(hWnd, hdc, ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_HSCROLL:
    {
        int xDelta = 0 , xNewPos = 0, yDelta = 0;

        int scrollId = LOWORD(wParam);

        switch (scrollId)
        {

        case SB_PAGEUP:
            xNewPos = xS - 50;
            break;
        case SB_PAGEDOWN:
            xNewPos = xS + 50;
            break;
        case SB_LINEUP:
            xNewPos = xS - 5;
            break;
        case SB_LINEDOWN:
            xNewPos = xS + 5;
            break; 
        case SB_THUMBPOSITION:
            xNewPos = HIWORD(wParam);
            break;

        default:
            xNewPos = xS;
        }

       
        xNewPos = max(0, xNewPos);
        xNewPos = min(xMaxScroll, xNewPos);


        if (xNewPos == xS)
            break;

        fScroll = TRUE;
        xDelta = xNewPos - xS;

        xS = xNewPos;

        ScrollWindowEx(hWnd, -xDelta, -yDelta, (CONST RECT*) NULL,
            (CONST RECT*) NULL, (HRGN)NULL, (PRECT)NULL,
            SW_INVALIDATE);
        UpdateWindow(hWnd);
        OnScrollPositionUpdated();

        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        si.nPos = xS;
        SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

        break;
    }

    case WM_SIZE:
    {
        int xNewSize, yNewSize;

        InitWindowContext(&this->__wndClass.context, hWnd, 0, 0);

        xNewSize = LOWORD(lParam);
        yNewSize = HIWORD(lParam);

        if (fBlt)
            fSize = TRUE;

        xMaxScroll = max(this->mainImage->width - xNewSize, 0);
        xS = min(xS, xMaxScroll);
        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        si.nMin = xMinScroll;
        si.nMax = this->mainImage->width;
        si.nPage = xNewSize;
        si.nPos = xS;
        SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
        yMaxScroll = max(this->mainImage->height - yNewSize, 0);
        yS = min(yS, yMaxScroll);
        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        si.nMin = yMinScroll;
        si.nMax = this->mainImage->height;
        si.nPage = yNewSize;
        si.nPos = yS;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

        break;
    }

    case WM_VSCROLL:
    {
        int xDelta = 0, yDelta = 0, yNewPos = 0;    // new position 

        switch (LOWORD(wParam))
        {
           
        case SB_PAGEUP:
            yNewPos = yS - 50;
            break;
        case SB_PAGEDOWN:
            yNewPos = yS + 50;
            break;
        case SB_LINEUP:
            yNewPos = yS - 5;
            break;
        case SB_LINEDOWN:
            yNewPos = yS + 5;
            break;
        case SB_THUMBPOSITION:
            yNewPos = HIWORD(wParam);
            break;

        default:
            yNewPos = yS;
        }

        
        yNewPos = max(0, yNewPos);
        yNewPos = min(yMaxScroll, yNewPos);

        if (yNewPos == yS)
            break;

       
        fScroll = TRUE;

       
        yDelta = yNewPos - yS;

        
        yS = yNewPos;

        ScrollWindowEx(hWnd, -xDelta, -yDelta, (CONST RECT*) NULL,
            (CONST RECT*) NULL, (HRGN)NULL, (PRECT)NULL,
            SW_INVALIDATE);
        UpdateWindow(hWnd);
        OnScrollPositionUpdated();

        si.cbSize = sizeof(si);
        si.fMask = SIF_POS;
        si.nPos = yS;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

static LRESULT OnMessage(CanvasWindow* window, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == ID_32778) {
        IT_Task6(this->mainImage);
        InvalidateRect(this->__wndClass.context.hWnd, 0, 0);
        int size = MAIN_WINDOW_WIDTH + (MAIN_WINDOW_HEIGHT << 16);
        fBlt = 0;
        return MyWndProc(hWnd, WM_SIZE, 0, size);
    }
    return MyWndProc(hWnd, message, wParam, lParam);
}

static ATOM RegisterWnd(CanvasWindow* window, HINSTANCE hInstance) {

    backGroundBrush = CreateSolidBrush(0xeeeeee);

    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MyWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(0, IDC_CROSS);
    wcex.hbrBackground = backGroundBrush;
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = window->__wndClass.winClassName;
    wcex.hIconSm = 0;

    return RegisterClassExA(&wcex);
}

static void OnDispose(CanvasWindow* window) {
    this = 0;
    DeleteObject(backGroundBrush);
    DisposeObject(window->mainImage);
    DisposeObject(window->imageBuffer);
    free(window->mainImage);
    free(window->imageBuffer);
    free(window->paintTool);
}



IWindowClass* GetCanvasWindow(CanvasStatusWindow* statusRef)
{

    if (this != 0) {
        return this;
    }

    CanvasWindow* canvasWindow =
        calloc(1, sizeof(CanvasWindow));
    if (canvasWindow == 0) {
        return debugMemError();
    }
    WINDOWCLASS_SUPER(CanvasWindow, canvasWindow);
    OverrideWindowClassRegister(CanvasWindow, RegisterWnd);
    OverrideWindowClassOnMessage(CanvasWindow, OnMessage);
    OverrideWindowClassCreate(CanvasWindow, OnCreate);
    OverrideObjectDispose(CanvasWindow, OnDispose);
    
    canvasWindow->statusRef = statusRef;
    canvasWindow->mainImage = 0;
    canvasWindow->imageBuffer = NewPixelBuffer();
    canvasWindow->toolType = PAINT_SELECT_TOOL;
    canvasWindow->paintTool = calloc(1, sizeof(PaintTool));
    if (canvasWindow->paintTool == 0) 
        return debugMemError();
    UpdatePaintTool(canvasWindow->paintTool, 1, 0xffffff, 0);

    this = canvasWindow;

    return canvasWindow;
}

void CanvasOnSaveImage(CanvasWindow* canvasWindow)
{
    ImageLoader_Save(canvasWindow->mainImage, L"output_test.bmp", canvasWindow);
}

