#pragma once

#include "ImageTransform.h"

void ImageScale_SimpleQuad(PixelBuffer* buffer, RECT* region, float scale);
void ImageScale_SimpleRect(PixelBuffer* buffer, RECT* region, float scaleX, float scaleY);