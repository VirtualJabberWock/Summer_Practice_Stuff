
#include "framework.h"
#include "PaintApp.h"

#include "util/win/Context.h"
#include "util/win/Dialog.h"
#include "util/win/IWindowClass.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

WindowContext glMainWindowContext;              // Контекст главного меню

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

#include "util/win_classes/CanvasStatusWindow.h"
#include "util/win_classes/CanvasWindow.h"
#include "util/win_classes/PaintUtilsWindow.h"

CanvasStatusWindow* canvasStatusWindow;
CanvasWindow* canvasWindow;
PaintUtilsWindow* utilsWindow;

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

void OnInit() {
    IWindowCreateAndShow(canvasStatusWindow, &glMainWindowContext);
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


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, 
       WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 0, 0, hInstance, 0);

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
            case ID_32778:
                IWindowSendMessage(canvasWindow, canvasWindow->__wndClass.context.hWnd,ID_32778, 0, 0);
            case IDM_SAVE_IMAGE:
                CanvasOnSaveImage(canvasWindow);
                break;
            case IDM_OPEN_IMAGE:
                fileName = WinOpenFileDialog(&glMainWindowContext);
                if (fileName != 0) {
                    if (canvasWindow->mainImage != 0) {
                        DisposeObject(canvasWindow->mainImage);
                        free(canvasWindow->mainImage);
                    }
                    int filenameLength = lstrlenW(fileName);
                    wchar_t* a = malloc((filenameLength + 1) * sizeof(wchar_t));
                    if (a == 0) return debugMemError();
                    for (int i = 0; i < filenameLength; i++) {
                        a[i] = fileName[i];
                    }
                    a[filenameLength] = 0;
                    canvasWindow->mainImage = ImageLoader_LoadBitmap(a, &canvasWindow->__wndClass);

                }
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
