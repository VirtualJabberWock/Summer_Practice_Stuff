#pragma once

#include "../Theme.h"

#define NormalColor(color) (  ((color & 0xff) << 16) | (color & 0x00ff00) | ((color & 0xff0000) >> 16)    )
#define SetBrush(brushName, color) SetThemeBrush(#brushName, CreateSolidBrush(NormalColor(color)))
#define SetBrushEx(brushName, brush) SetThemeBrush(#brushName, brush)

void applyLight() 
{
	SetBrush(Theme_B_Canvas, 0xF7F9F9);
	SetBrush(Theme_B_ToolBar, 0xE0EEEE);
	SetBrush(Theme_B_StatusBar, 0xffffff);
}

Theme* CreateLightTheme() {
	return NewTheme("LightTheme", applyLight);
}