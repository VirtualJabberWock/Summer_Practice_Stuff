#pragma once

#include "../Theme.h"

void applyDark() 
{
	SetBrush(Theme_B_Canvas, 0x282828);
	SetBrush(Theme_B_ToolBar, 0x535353);
	SetBrush(Theme_B_StatusBar, 0xffffff);

	SetBrush(Theme_B_button, 0x92CCC3);
	SetBrush(Theme_B_buttonFocus, 0x819AD5);
	SetBrush(Theme_B_buttonPressed, 0x3EE29B);

	SetPen(Theme_P_button, 0x7EB3AA);
	SetPen(Theme_P_buttonFocus, 0x496BBB);
	SetPen(Theme_P_buttonPressed, 0x2BC280);
}

Theme* CreateDarkTheme() {
	return NewTheme("DarkTheme", "dark", applyDark);
}
