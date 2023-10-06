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

	SetBrush(Theme_B_button, 0xA0C2C6);
	SetBrush(Theme_B_buttonFocus, 0x8FDDE4);
	SetBrush(Theme_B_buttonPressed, 0x3BBADD);

	SetPen(Theme_P_button, 0x5B7275);
	SetPen(Theme_P_buttonFocus, 0x6BCCD5);
	SetPen(Theme_P_buttonPressed, 0x2399BA);
}

Theme* CreateLightTheme() {
	return NewTheme("LightTheme", "light", applyLight);
}