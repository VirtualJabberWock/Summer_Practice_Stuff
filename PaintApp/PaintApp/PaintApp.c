
#include "framework.h"
#include "PaintApp.h"

#include "app/win/Context.h"
#include "app/win/Dialog.h"
#include "app/win/IWindowClass.h"

#define DEBUG_CONSOLE 1

#define MAX_LOADSTRING 100


HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            

WindowContext glMainWindowContext;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

#include "app/win_classes/CanvasStatusWindow.h"
#include "app/win_classes/CanvasWindow.h"
#include "app/win_classes/PaintUtilsWindow.h"

#include "app/appearance/Theme.h"

CanvasStatusWindow* canvasStatusWindow;
CanvasWindow* canvasWindow;
PaintUtilsWindow* utilsWindow;

void ApplySelectedTheme();

void RegisterAllClasses(HINSTANCE h) {

    canvasStatusWindow = NewCanvasStatusWindow();
    canvasWindow = GetCanvasWindow(canvasStatusWindow);
    utilsWindow = NewPaintUtilsWindow(canvasWindow);
    IWindowRegister(canvasStatusWindow, h);
    IWindowRegister(canvasWindow, h);
    IWindowRegister(utilsWindow, h);
}

void OnCreate() {
    
}

void OnInit() 
{
#if DEBUG_CONSOLE
    AllocConsole();
    SetConsoleTitleA("Debug info");
    freopen_s(stdout, "CONOUT$", "w", stdout);
    printf("# Console created at PaintApp.OnInit()\n");
    printf("# For disable: set DEBUG_CONSOLE to 0 in PaintApp.c\n");
    printf("\n");
#endif
    //IWindowCreateAndShow(canvasStatusWindow, &glMainWindowContext);
    IWindowCreateAndShow(canvasWindow, &glMainWindowContext);
    IWindowCreateAndShow(utilsWindow, &glMainWindowContext);
}

void DisposeAllClasses() {
    if (canvasStatusWindow != 0) {
        DisposeObject(canvasStatusWindow);
        free(canvasStatusWindow);
        canvasStatusWindow = 0;
    }
    if (canvasWindow != 0) {
        DisposeObject(canvasWindow);
        free(canvasWindow);
        canvasWindow = 0;
    }
    if (utilsWindow != 0) {
        DisposeObject(utilsWindow);
        free(utilsWindow);
        utilsWindow = 0;
    }
}

void OnExit() {
    DisposeAllClasses();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    InitInternalThemes();
    ApplySelectedTheme();

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PAINTAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    RegisterAllClasses(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINTAPP));

    MSG msg;

    while (GetMessage(&msg, 0, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINTAPP));
    wcex.hCursor        = LoadCursor(0, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINTAPP);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

#include <dwmapi.h>

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, 
       WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, 0, 0, hInstance, 0);

   if (!hWnd)
   {
      return FALSE;
   }

   InitWindowContext(&glMainWindowContext, hWnd, &glMainWindowContext, hInst);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   OnInit();

   return TRUE;
}

void OnResize() {

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPCWSTR fileName = 0;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_LIGHT_THEME:
                ApplyTheme(GetLoadedTheme("LightTheme"));
                break;
            case IDM_DARK_THEME:
                ApplyTheme(GetLoadedTheme("DarkTheme"));
                break;
            case ID_TASK6:
                IWindowSendMessage(canvasWindow, canvasWindow->__wndClass.context.hWnd,ID_TASK6, 0, 0);
            case IDM_SAVE_IMAGE:
                CanvasOnSaveImage(canvasWindow);
                break;
            case IDM_OPEN_IMAGE:
                fileName = WinOpenFileDialog(&glMainWindowContext);
                if (fileName != 0) {
                    int filenameLength = lstrlenW(fileName);
                    wchar_t* a = malloc((filenameLength + 1) * sizeof(wchar_t));
                    if (a == 0) return debugMemError();
                    for (int i = 0; i < filenameLength; i++) {
                        a[i] = fileName[i];
                    }
                    a[filenameLength] = 0;
                    if (canvasWindow->mainImage != 0) {
                        DisposeObject(canvasWindow->mainImage);
                        free(canvasWindow->mainImage);
                    }
                    canvasWindow->mainImage = ImageLoader_LoadBitmap(a, &canvasWindow->__wndClass);
                }
                InvalidateRect(0, 0, 0);
                break;
            case IDM_ABOUT:
                MessageBoxAFormat("Window Info", "Width: %d, Height: %d",
                    glMainWindowContext.width, glMainWindowContext.height);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    break;
    case WM_CREATE: OnCreate(); break;
    case WM_MOUSEMOVE:
        break;
    case WM_DESTROY:
        OnExit();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void ApplySelectedTheme() {
    ApplyTheme(GetLoadedTheme("LightTheme"));
}

void RepaintAppView() {
    if (canvasStatusWindow != 0) {
        InvalidateRect(canvasStatusWindow->__wndClass.context.hWnd, 0, 1);
    }
    if (canvasWindow != 0) {
        InvalidateRect(canvasWindow->__wndClass.context.hWnd, 0, 1);
        canvasWindow->isCanvasInvalidated = 1;
    }
    if (utilsWindow != 0) {
        InvalidateRect(utilsWindow->__wndClass.context.hWnd, 0, 1);
    }
}