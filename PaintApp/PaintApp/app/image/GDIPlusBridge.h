#pragma once
#include <wtypes.h>

typedef __int64 GpToken;
typedef __int64 GpBitmap;
typedef __int64 GpImage;

GpToken GDI_Init();
void GDI_Shutdown(GpToken token);
HBITMAP GDI_LoadImage(wchar_t* filename);
int GDI_SaveImage(wchar_t* filename, HBITMAP bitmap, int format);
