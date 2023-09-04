#pragma once

#include "../Theme.h"

#define NormalColor(color) (  ((color & 0xff) << 16) | (color & 0x00ff00) | ((color & 0xff0000) >> 16)    )
#define SetBrush(brushName, color) SetThemeBrush(#brushName, CreateSolidBrush(NormalColor(color)))
#define SetBrushEx(brushName, brush) SetThemeBrush(#brushName, brush)

void applyDark() 
{
	SetBrush(Theme_B_Canvas, 0x282828);
	SetBrush(Theme_B_ToolBar, 0x535353);
	SetBrush(Theme_B_StatusBar, 0xffffff);
}

Theme* CreateDarkTheme() {
	return NewTheme("DarkTheme", applyDark);
}
