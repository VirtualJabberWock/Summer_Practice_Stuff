#include "TextureUtil.h"
#include <stdio.h>

static UINT32 alpha_register = 0;
static UINT32 replace_register = 0;

static void replacer(UINT32* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) 
{
    for (int i = yFrom; i < yTo; i++) {
        for (int j = xFrom; j < xTo; j++) {
            if (pixels[i * width + j] == alpha_register) {
                pixels[i * width + j] = replace_register;
            }
        }
    }
}

HBITMAP ReplaceAlphaChannel(HBITMAP texture, UINT32 replace_color, UINT32 alpha_channel)
{
    alpha_register = alpha_channel;
    replace_register = replace_color;
    return CallIteratorOnPixels(texture, replacer, 0, 0, 0);
}

UINT32 GetBrushColor(HBRUSH brush)
{
    static COLORREF lbColor;
    LOGBRUSH br = { 0 };

    if (GetObject(brush, sizeof(br), &br))
    {
        return COLORREF2RGB(br.lbColor);
    }
    printf("[WARNING] [GetBrushColor] can't get brush color");
    return 0;
}
