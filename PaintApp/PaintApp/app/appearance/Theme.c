#include "Theme.h"
#include <string.h>
#include <Windows.h>

DEFINE_OBJ_TYPE(Theme);

void DisposeTheme(Theme* theme)
{
	if (theme->name != 0) {
		free(theme->name);
		theme->name = 0;
	}
}

Theme* NewTheme(const char* name, void_func applyFunc)
{
	Theme* theme = calloc(1, sizeof(Theme));

	if (theme == 0) {
		return 0;
	}

	OBJECT_SUPER(Theme, theme);

	OverrideObjectDispose(Theme, DisposeTheme);

	theme->apply = applyFunc;
	int nameLength = strlen(name);
	theme->name = calloc(nameLength + 1, sizeof(char));
	if (theme->name == 0) {
		return debugMemError();
	}
	memcpy_s(theme->name, nameLength, name, nameLength); //TODO
	((char*)theme->name)[nameLength] = 0;
	return theme;
}

void InitInternalThemes()
{
	Theme_B_Main = CreateSolidBrush(0x191919);

	Theme_B_button = CreateSolidBrush(0x191919);
	Theme_B_buttonFocus = CreateSolidBrush(0x191919);
	Theme_B_buttonPressed = CreateSolidBrush(0x191919);
	Theme_B_buttonDisabled = CreateSolidBrush(0x191919);

	Theme_P_button = CreatePen(PS_SOLID, 1, 0x191919);
	Theme_P_buttonFocus = CreatePen(PS_SOLID, 1, 0x191919);
	Theme_P_buttonPressed = CreatePen(PS_SOLID, 1, 0x191919);
	Theme_P_buttonDisabled = CreatePen(PS_SOLID, 1, 0x191919);

	Theme_B_ToolBar = CreateSolidBrush(0xffffff);
	Theme_B_Canvas = CreateSolidBrush(0xffffff);
	Theme_B_StatusBar = CreateSolidBrush(0xffffff);

	AddTheme(CreateLightTheme());
	AddTheme(CreateDarkTheme());
}

#include "../container/map/HashMap.h"
#include "../container/Map.h"
#include "../core/String.h"

static IMap* themeMap = 0;

static IMap* getThemeMap() {
	if (themeMap == 0) {
		themeMap = NewHashMap(); // <- HashMap
	}
	return themeMap;
}

void IterateThemeNames(ObjectIterator iterator)
{
	IMap* map = getThemeMap();
	map->mapMethods->iterateKeys(map, iterator);
}

Theme* GetLoadedTheme(const char* name)
{
	IMap* map = getThemeMap();
	return mapGetByString(map, name);
}

void AddTheme(Theme* theme)
{
	IMap* map = getThemeMap();
	map->mapMethods->emplace(map, NewString(theme->name), theme);
}

DLL_EXPORT void SetThemeBrush(const char* fullBrushName, HBRUSH brush)
{
	HBRUSH* brushRef = GetProcAddress(GetModuleHandle(NULL), fullBrushName);
	if (*brushRef != 0) {
		DeleteObject(*brushRef);
	}
	*brushRef = brush;
}

void SetThemePen(const char* fullPenName, HPEN pen)
{
	HPEN* penRef = GetProcAddress(GetModuleHandle(NULL), fullPenName);
	if (*penRef != 0) {
		DeleteObject(*penRef);
	}
	*penRef = pen;
}

void DisposeThemes()
{
	if (themeMap != 0) {
		DisposeObject(themeMap);
		free(themeMap);
		themeMap = 0;
	}
}

#include "../../PaintApp.h"

void ApplyTheme(Theme* theme)
{
	theme->apply();
	RepaintAppView();
}

