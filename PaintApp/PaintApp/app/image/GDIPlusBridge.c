#include "GDIPlusBridge.h"
#include <stdio.h>
#include "../core/DebugUtil.h"

typedef (*wrapped_func)(void* arg, ...);
typedef (*wrapped_no_arg_func)();

wrapped_func _GdiplusStartup = 0;
wrapped_func _GdiplusShutdown = 0;
wrapped_func _GdipCreateBitmapFromFile = 0;
wrapped_func _GdipCreateHBITMAPFromBitmap = 0;
wrapped_func _GdipCreateBitmapFromHBITMAP = 0;
wrapped_func _GdipLoadImageFromFile = 0;
wrapped_func _GdipSaveImageToFile = 0;
wrapped_func _GdipDisposeImage = 0;

typedef struct GdiplusStartupInput_ {
    UINT32         GdiplusVersion;
    wrapped_func   DebugEventCallback;
    BOOL           SuppressBackgroundThread;
    BOOL           SuppressExternalCodecs;
    wrapped_func __call;
}  GdiplusStartupInput;

__int64 saved_token = 0;

GpToken GDI_Init()
{
    if (saved_token != 0) 
    {
        return saved_token;
    }

    HMODULE lib = LoadLibraryA("gdiplus.dll");
    if (lib == 0) {
        return debugFatalError("Can't load library 'gdiplus.dll'");
    }
    _GdiplusStartup = GetProcAddress(lib, "GdiplusStartup");
    _GdiplusShutdown = GetProcAddress(lib, "GdiplusShutdown");
    _GdipCreateBitmapFromFile = GetProcAddress(lib, "GdipCreateBitmapFromFile");
    _GdipCreateHBITMAPFromBitmap = GetProcAddress(lib, "GdipCreateHBITMAPFromBitmap");
    _GdipCreateBitmapFromHBITMAP = GetProcAddress(lib, "GdipCreateBitmapFromHBITMAP");
    _GdipLoadImageFromFile = GetProcAddress(lib, "GdipLoadImageFromFile");
    _GdipSaveImageToFile = GetProcAddress(lib, "GdipSaveImageToFile");
    _GdipDisposeImage = GetProcAddress(lib, "GdipDisposeImage");

    GdiplusStartupInput input;
    input.GdiplusVersion = 1;
    input.DebugEventCallback = NULL;
    input.SuppressBackgroundThread = FALSE;
    input.SuppressExternalCodecs = FALSE;
    GpToken token = 0;
    GpToken* ptr = &token;
    int t = _GdiplusStartup(ptr, &input, NULL);
    saved_token = token;
    return token;
}

void GDI_Shutdown(GpToken token)
{
    if (token == 0) _GdiplusShutdown(saved_token);
    else _GdiplusShutdown(token);
}

HBITMAP GDI_LoadImage(wchar_t* filename)
{
    GpBitmap* result = 0;
    _GdipCreateBitmapFromFile(filename, &result, 0, 0);
    HBITMAP hBitmap = 0;
    _GdipCreateHBITMAPFromBitmap(result, &hBitmap, 0);
    _GdipDisposeImage(result);
    return hBitmap;
}

#include "ImageLoader.h"

/*
gGIF_GUID = {557CF402-1A04-11D3-9A73-0000F81EF32E}
gPNG_GUID = {557CF406-1A04-11D3-9A73-0000F81EF32E}
gJPG_GUID = {557CF401-1A04-11D3-9A73-0000F81EF32E}
gBMP_GUID = {557CF400-1A04-11D3-9A73-0000F81EF32E}
gTIF_GUID = {557CF405-1A04-11D3-9A73-0000F81EF32E}
*/

int GDI_SaveImage(wchar_t* filename, HBITMAP bitmap, ImageFileFormat format)
{
    GUID guid = { 0 };
    char guid_tail[8] = { 0x9a, 0x73, 00, 00, 0xf8, 0x1e, 0xf3, 0x2e };
    memcpy(guid.Data4, guid_tail, 8);
    guid.Data2 = 0x1a04;
    guid.Data3 = 0x11D3;
    switch (format)
    {
    case IMAGE_FORMAT_BMP:
        guid.Data1 = 0x557CF400; break;
    case IMAGE_FORMAT_PNG:
        guid.Data1 = 0x557CF406; break;
    case IMAGE_FORMAT_JPG:
        guid.Data1 = 0x557CF401; break;
    default:
        debugFatalError("[ImageLoader+GDI] wrong image file format");
        break;
    }
    GpBitmap* result;
    _GdipCreateBitmapFromHBITMAP(bitmap, 0, &result);
    int status = _GdipSaveImageToFile(result, filename, &guid, 0);
    _GdipDisposeImage(result);
    return status;
}




