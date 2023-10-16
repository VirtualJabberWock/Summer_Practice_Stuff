
#include "Dialog.h"
#include <Windows.h>

#include <stdio.h>
#include <varargs.h>

void InitMouseMenu(MouseMenu* menu)
{
	menu->menu = CreatePopupMenu();
}

void AddOptionToMouseMenu(MouseMenu* menu, LPCWSTR* name, int id, int isActive)
{
	int disabledFlag = (isActive) ? 0 : MF_DISABLED;
	InsertMenuW(menu->menu, 0, MF_BYPOSITION | MF_STRING | disabledFlag, id, name);
}

int ShowMouseMenu(MouseMenu* menu, HWND hWnd, int x, int y)
{
	SetForegroundWindow(hWnd);
	POINT p = { x, y };
	ClientToScreen(hWnd, &p);
	return TrackPopupMenu(menu->menu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD,
		p.x, p.y, 0, hWnd, NULL
	);
}

void DestroyMouseMenu(MouseMenu* menu)
{
	return DestroyMenu(menu->menu);
}


int MessageBoxAFormat(const char* title, const char* format, ...)
{
	va_list args;
	va_start(args, format);

	char* buffer = calloc(5000, sizeof(char));
	if (buffer == 0) {
		return -1;
	}
	vsprintf_s(buffer, 5000, format, args);
	int status = MessageBoxA(0, buffer, title, 0x40L);
	free(buffer);
	return status;
}

LPCWSTR WinOpenFileDialog(WindowContext* context)
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260] = { 0 };       // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = context->hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Image file\0*.bmp\0PNG\0*.png\0JPEG\0*.jpeg";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		LPCWSTR a = ofn.lpstrFile;

		return a;
	}
	return 0;
}
