#include "PaintUtilsWindow.h"

#include "..\win\SimpleGraphics.h"
#include "MainFrameChunks.h"
#include "..\win\ButtonUI.h"
#include "..\paint\ToolButtonUI.h"
#include "../paint/PaintToolsLoader.h"

DEFINE_WINDOWCLASS_TYPE(PaintUtilsWindow);

PaintUtilsWindow* this = 0;

static HPEN toolIconSelect = 0;

static void ToolButtonDraw(ToolButton* button, HDC hdc, HWND hWnd) {
    
    if (this == 0) return;

    UI_Button_BasicDraw(button, hdc, hWnd);
    
    // extra hook between
    if (isPaintToolIsStub(button->toolReference)) {
        GET_EXPAND_RECT(button->btn.rect, selectIconR, -20, -5, -20, -5);
        DrawRect(hdc, &__selectIconR, toolIconSelect);
        return;
    }
   
    UI_ToolButton_BasicDraw(button, this->canvasWindow->paintToolProperties, hdc, hWnd);
}

static void ColorButtonDraw(Button* button, HDC hdc, HWND hWnd)
{
    button->state = BUTTON_STATE_ACTIVE;
    UI_Button_BasicDraw(button, hdc, hWnd);
    GET_EXPAND_RECT(button->rect, rIconR, -8, -5, -8, -5);
    FillRect(hdc, &__rIconR, this->canvasWindow->paintToolProperties->brush);
    DrawRect(hdc, &__rIconR, 0);
}

static void OnToolButtonClick(ToolButton* button) {

    if (this == 0) return;

    if (isPaintToolIsStub(button->toolReference)) {
        this->canvasWindow->toolType = PAINT_SELECT_TOOL;
    }
    else {
        this->canvasWindow->paintTool = button->toolReference;
        this->canvasWindow->toolType = PAINT_DRAW_TOOL;
    }

    if (button->btn.state != BUTTON_STATE_SELECTED) {
        for (int i = 0; i < this->toolButtons->size; i++) {
            Button* btn = this->toolButtons->data[i];
            btn->state = BUTTON_STATE_ACTIVE;
        }
        button->btn.state = BUTTON_STATE_SELECTED;
    }
    else {
        button->btn.state = BUTTON_STATE_FOCUS;
        this->canvasWindow->toolType = PAINT_NO_TOOL;
    }
    for (int i = 0; i < this->toolButtons->size; i++) {
        Button* btn = this->toolButtons->data[i];
        btn->rectValidation = 0;
        UI_ButtonNotify(btn, btn->rect.left+1, btn->rect.top+1, -1, this->__wndClass.context.hWnd);
    }
}

static void OnColorButtonClick(Button* btn) {


    COLORREF acrCustClr[16] = {0};
    CHOOSECOLOR cc;
    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = this->canvasWindow->__wndClass.context.hWnd; // owner window, allowed to be NULL if no owner
    cc.lpCustColors = acrCustClr;
    cc.rgbResult = this->canvasWindow->paintToolProperties->color;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColorW(&cc)) {
        UpdatePaintToolProperties(
            this->canvasWindow->paintToolProperties,
            this->canvasWindow->paintToolProperties->width,
            cc.rgbResult,
            this->canvasWindow->paintToolProperties->isFill
        );
    }

    UI_ButtonNotify(btn, btn->rect.left + 1, btn->rect.top + 1, -1, this->__wndClass.context.hWnd);
}

static HWND OnCreate(PaintUtilsWindow* window, WindowContext* optParent) {
    if (optParent == 0) return 0;

    toolIconSelect = CreatePen(PS_DOT, 1, 0x444444);
    
    HWND h = CreateWindowExA(
        0,
        window->__wndClass.winClassName, "",
        WS_CHILD,
        0, 0, optParent->width, UTILS_HEIGHT,
        optParent->hWnd,
        0, optParent->hInst,
        0
    );

    return h;
}

#include "../appearance/Theme.h"

static LRESULT MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (this == 0) {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    int mx = LOWORD(lParam);
    int my = HIWORD(lParam);

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        FillRect(hdc, &this->__wndClass.context.rect, Theme_B_ToolBar);
        for (int i = 0; i < this->buttons->size; i++) {
            UI_ButtonDraw(this->buttons->data[i], hdc, hWnd);
        }
        for (int i = 0; i < this->toolButtons->size; i++) {
            UI_ButtonDraw(this->toolButtons->data[i], hdc, hWnd);
        }
        EndPaint(hWnd, &ps);
        break;
    }
    break;
    case WM_LBUTTONDOWN:
        for (int i = 0; i < this->buttons->size; i++) {
            UI_ButtonNotify(this->buttons->data[i], mx, my, 1, hWnd);
        }
        for (int i = 0; i < this->toolButtons->size; i++) {
            UI_ButtonNotify(this->toolButtons->data[i], mx, my, 1, hWnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE:
        for (int i = 0; i < this->buttons->size; i++) {
            UI_ButtonNotify(this->buttons->data[i], mx, my, 0, hWnd);
        }
        for (int i = 0; i < this->toolButtons->size; i++) {
            UI_ButtonNotify(this->toolButtons->data[i], mx, my, 0, hWnd);
        }
        break;
    case WM_ERASEBKGND:
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);;
}

static LRESULT OnMessage(PaintUtilsWindow* window, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return MyWndProc(hWnd, message, wParam, lParam);
}

static ATOM RegisterWnd(PaintUtilsWindow* window, HINSTANCE hInstance) {

    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MyWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(0xe3e1de);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = window->__wndClass.winClassName;
    wcex.hIconSm = 0;

    return RegisterClassExA(&wcex);
}

static void OnDispose(PaintUtilsWindow* window)
{
    DisposeObject(window->buttons);
    DisposeObject(window->toolButtons);
    free(window->buttons);
    free(window->toolButtons);
    window->buttons = 0;
    window->toolButtons = 0;
}

IWindowClass* NewPaintUtilsWindow(CanvasWindow* canvasWindow)
{

    if (this != 0) return this;

    PaintUtilsWindow* utilsWindow =
        calloc(1, sizeof(PaintUtilsWindow));
    if (utilsWindow == 0) {
        return debugMemError();
    }

    PaintToolsLoaderInit();

    WINDOWCLASS_SUPER(PaintUtilsWindow, utilsWindow);
    OverrideWindowClassRegister(PaintUtilsWindow, RegisterWnd);
    OverrideWindowClassOnMessage(PaintUtilsWindow, OnMessage);
    OverrideWindowClassCreate(PaintUtilsWindow, OnCreate);
    OverrideObjectDispose(PaintUtilsWindow, OnDispose);
    utilsWindow->canvasWindow = canvasWindow;
    utilsWindow->buttons = NewObjectVector(8, 0);
    utilsWindow->toolButtons = NewObjectVector(8, 0);

    ObjectV_PushBack(utilsWindow->toolButtons, NewToolButton(
        16, 15, 96, 32, OnToolButtonClick, ToolButtonDraw, "SelectTool"
    ));
    ObjectV_PushBack(utilsWindow->toolButtons, NewToolButton(
        32 + 96, 15, 64, 32, OnToolButtonClick, ToolButtonDraw, "RectTool"
    ));
    ObjectV_PushBack(utilsWindow->toolButtons, NewToolButton(
        208, 15, 64, 32, OnToolButtonClick, ToolButtonDraw, "LineTool"
    ));
    ObjectV_PushBack(utilsWindow->buttons, NewButton(
        MAIN_WINDOW_WIDTH - 208, 15, 128, 32, OnColorButtonClick, ColorButtonDraw, 1
    ));

    ((Button*)(utilsWindow->buttons->data[0]))->state = BUTTON_STATE_SELECTED;
    this = utilsWindow;
    return utilsWindow;
}
