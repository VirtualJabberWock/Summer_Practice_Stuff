#include "PaintUtilsWindow.h"


#include "..\win\SimpleGraphics.h"

DEFINE_WINDOWCLASS_TYPE(PaintUtilsWindow);

PaintUtilsWindow* this = 0;

static HPEN toolIconSelect = 0;
static HPEN toolIconRectEdge = 0;

static void ToolButtonDraw(ButtonUI* button, HDC hdc, HWND hwnd) {
    
    if (this == 0) return;

    HBRUSH t = this->canvasWindow->paintTool->brush;

    UI_Button_BasicDraw(button, hdc, hwnd);
    GET_EXPAND_RECT(button->rect, selectIconR, -20, -5, -20, -5);
    GET_EXPAND_RECT(button->rect, rIconR, -8, -5, -8, -5);
    switch (button->tag)
    {
    case PAINT_SELECT_TOOL:
        DrawRect(hdc, &__selectIconR, toolIconSelect);
        break;
    case PAINT_SQUARE_TOOL:
        DrawRect(hdc, &__rIconR, toolIconRectEdge);
        break;
    case PAINT_LINE_TOOL:
        DrawLine(hdc, __rIconR.left, __rIconR.top, __rIconR.right, __rIconR.bottom, toolIconRectEdge);
        break;
    case PAINT_COLOR_TOOL:
        FillRect(hdc, &__rIconR, t);
        DrawRect(hdc, &__rIconR, 0);
        break;
    default:
        break;
    }
}

static void OnToolButtonClick(ButtonUI* button) {
    if (this == 0) return;
    if (button->state != BUTTON_STATE_SELECTED) {
        for (int i = 0; i < this->buttons->size; i++) {
            ButtonUI* btn = this->buttons->data[i];
            btn->state = BUTTON_STATE_ACTIVE;
        }
        button->state = BUTTON_STATE_SELECTED;
        this->canvasWindow->toolType = button->tag;
    }
    else {
        button->state = BUTTON_STATE_FOCUS;
        this->canvasWindow->toolType = PAINT_NO_TOOL;
    }
    for (int i = 0; i < this->buttons->size; i++) {
        ButtonUI* btn = this->buttons->data[i];
        btn->rectValidation = 0;
        UI_ButtonNotify(btn, btn->rect.left+1, btn->rect.top+1, -1, this->__wndClass.context.hWnd);
    }
}

static void OnColorButtonClick(ButtonUI* btn) {


    COLORREF acrCustClr[16] = {0};
    CHOOSECOLOR cc;
    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = this->canvasWindow->__wndClass.context.hWnd; // owner window, allowed to be NULL if no owner
    cc.lpCustColors = acrCustClr;
    cc.rgbResult = this->canvasWindow->paintTool->color;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColorW(&cc)) {
        UpdatePaintTool(
            this->canvasWindow->paintTool,
            this->canvasWindow->paintTool->width,
            cc.rgbResult,
            this->canvasWindow->paintTool->isFill
        );
    }

    UI_ButtonNotify(btn, btn->rect.left + 1, btn->rect.top + 1, -1, this->__wndClass.context.hWnd);
}

static HWND OnCreate(PaintUtilsWindow* window, WindowContext* optParent) {
    if (optParent == 0) return 0;

    toolIconSelect = CreatePen(PS_DOT, 1, 0x444444);
    toolIconRectEdge = CreatePen(PS_SOLID, 2, 0);
    
    HWND h = CreateWindowExA(
        0,
        window->__wndClass.winClassName, "",
        WS_CHILD | WS_BORDER,
        0, 0, optParent->width, UTILS_HEIGHT,
        optParent->hWnd,
        0, optParent->hInst,
        0
    );

    return h;
}

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
        for (int i = 0; i < this->buttons->size; i++) {
            UI_ButtonDraw(this->buttons->data[i], hdc, hWnd);
        }
        EndPaint(hWnd, &ps);
        break;
    }
    break;
    case WM_LBUTTONDOWN:
        for (int i = 0; i < this->buttons->size; i++) {
            UI_ButtonNotify(this->buttons->data[i], mx, my, 1, hWnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE:
        for (int i = 0; i < this->buttons->size; i++) {
            UI_ButtonNotify(this->buttons->data[i], mx, my, 0, hWnd);
        }
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
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = window->__wndClass.winClassName;
    wcex.hIconSm = 0;

    return RegisterClassExA(&wcex);
}

static void OnDispose(PaintUtilsWindow* window) {
    DisposeObject(window->buttons);
    free(window->buttons);
    window->buttons = 0;
}

IWindowClass* NewPaintUtilsWindow(CanvasWindow* canvasWindow)
{

    if (this != 0) return this;

    PaintUtilsWindow* utilsWindow =
        calloc(1, sizeof(PaintUtilsWindow));
    if (utilsWindow == 0) {
        return debugMemError();
    }

    WINDOWCLASS_SUPER(PaintUtilsWindow, utilsWindow);
    OverrideWindowClassRegister(PaintUtilsWindow, RegisterWnd);
    OverrideWindowClassOnMessage(PaintUtilsWindow, OnMessage);
    OverrideWindowClassCreate(PaintUtilsWindow, OnCreate);
    OverrideObjectDispose(PaintUtilsWindow, OnDispose);
    utilsWindow->canvasWindow = canvasWindow;
    utilsWindow->buttons = NewObjectVector(8, 0);
    ObjectV_PushBack(utilsWindow->buttons, NewButtonUI(
        16, 15, 96, 32, OnToolButtonClick, ToolButtonDraw, PAINT_SELECT_TOOL
    ));
    ObjectV_PushBack(utilsWindow->buttons, NewButtonUI(
        32+96, 15, 64, 32, OnToolButtonClick, ToolButtonDraw, PAINT_SQUARE_TOOL
    ));
    ObjectV_PushBack(utilsWindow->buttons, NewButtonUI(
        208, 15, 64, 32, OnToolButtonClick, ToolButtonDraw, PAINT_LINE_TOOL
    ));
    ObjectV_PushBack(utilsWindow->buttons, NewButtonUI(
        MAIN_WINDOW_WIDTH - 208, 15, 128, 32, OnColorButtonClick, ToolButtonDraw, PAINT_COLOR_TOOL
    ));

    ((ButtonUI*)(utilsWindow->buttons->data[0]))->state = BUTTON_STATE_SELECTED;
    this = utilsWindow;
    return utilsWindow;
}
