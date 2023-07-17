#pragma once
#include <Windows.h>

typedef struct tagMouseMenu {

	HMENU menu;

} MouseMenu;

void InitMouseMenu(MouseMenu* menu);
void AddOptionToMouseMenu(MouseMenu* menu, char* name, int id, int isActive);
int ShowMouseMenu(MouseMenu* menu, HWND hWnd, int x, int y);
void DestroyMouseMenu(MouseMenu* menu);

int MessageBoxAFormat(const char* title, const char* format, ...);
