#pragma once

#include "ImageTransform.h"
#include <minwindef.h>

HBITMAP ReplaceAlphaChannel(HBITMAP texture, UINT32 replace_color, UINT32 alpha_channel);

UINT32 GetBrushColor(HBRUSH brush);