#pragma once
#include "../core/Object.h"
#include "../container/ObjectVector.h"
#include <wtypes.h>

#define DLL_EXPORT __declspec(dllexport)

//             THEME BASE       

DLL_EXPORT HBRUSH Theme_B_Main;

DLL_EXPORT HBRUSH Theme_B_button;
DLL_EXPORT HBRUSH Theme_B_buttonFocus;
DLL_EXPORT HBRUSH Theme_B_buttonPressed;
DLL_EXPORT HBRUSH Theme_B_buttonDisabled;

DLL_EXPORT HBRUSH Theme_B_Canvas;
DLL_EXPORT HBRUSH Theme_B_ToolBar;
DLL_EXPORT HBRUSH Theme_B_StatusBar;

DLL_EXPORT HPEN Theme_P_button;
DLL_EXPORT HPEN Theme_P_buttonFocus;
DLL_EXPORT HPEN Theme_P_buttonPressed;
DLL_EXPORT HPEN Theme_P_buttonDisabled;

//

typedef void (*void_func)();

typedef struct ThemeTag {

	EXTENDS_OBJECT;

	const char* name;
	const char* resource_folder;
	void_func apply;

} Theme;

Theme* NewTheme(const char* name, const char* folder, void_func applyFunc);

void InitInternalThemes();

DLL_EXPORT void IterateThemeNames(ObjectIterator iterator);
DLL_EXPORT Theme* GetLoadedTheme(const char* name);
DLL_EXPORT void AddTheme(Theme* theme);

DLL_EXPORT void SetThemeBrush(const char* fullBrushName, HBRUSH brush);
DLL_EXPORT void SetThemePen(const char* fullPenName, HPEN pen);

void DisposeThemes();

/*
    INTERNAL THEMES:
*/

Theme* CreateLightTheme();
Theme* CreateDarkTheme();

void ApplyTheme(Theme* theme);
Theme* GetCurrentTheme();

/*
	Helpers
*/

#define NormalColor(color) (  ((color & 0xff) << 16) | (color & 0x00ff00) | ((color & 0xff0000) >> 16)    )
#define SetBrush(brushName, color) SetThemeBrush(#brushName, CreateSolidBrush(NormalColor(color)))
#define SetBrushEx(brushName, brush) SetThemeBrush(#brushName, brush)
#define SetPen(brushName, color) SetThemePen(#brushName, CreatePen(PS_SOLID, 1, NormalColor(color)))
#define SetPenEx(brushName, style, width, color) SetThemePen(#brushName, CreatePen(style, width, NormalColor(color)))

void InitApplicationContext(HINSTANCE inst);

DLL_EXPORT HBITMAP LoadImageResource(const char* id, Theme* theme);