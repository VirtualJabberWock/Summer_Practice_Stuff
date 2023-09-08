#include "ImageScaleUtil.h"
#include <stdio.h>

typedef unsigned char ubyte;

void ImageScale_SimpleQuad(PixelBuffer* buffer, RECT* region, float scale)
{

    if (scale > 5.0) {
        printf("[ImageScaleUtil] Very big scale '%f', reset to 5.0", scale);
        scale = 5.0;
    }

    int new_width = (int)(buffer->wSize * scale);
    int new_height = (int)(buffer->hSize * scale);

    UINT* scaledImage = calloc(new_width * new_height, sizeof(UINT));

    if (scaledImage == 0) {
        return debugMemError();
    }

    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            int _i = (int)((float)(i) / scale);
            int _j = (int)((float)(j) / scale);
            scaledImage[i * new_width + j] = buffer->pixels[_i * buffer->wSize + _j];
        }
    }

    free(buffer->pixels);

    buffer->pixels = scaledImage;
    buffer->wSize = new_width;
    buffer->hSize = new_height;
}

void ImageScale_SimpleRect(PixelBuffer* buffer, RECT* region, float scaleX, float scaleY)
{
    if (scaleX > 5.0 || scaleY > 5.0) {
        printf("[ImageScaleUtil] Very big scale, reset to 5.0");
    }

    scaleX = min(5.0, scaleX);
    scaleY = min(5.0, scaleY);

    int new_width = (int)(buffer->wSize * scaleX);
    int new_height = (int)(buffer->hSize * scaleY);

    UINT* scaledImage = calloc(new_width * new_height, sizeof(UINT));

    if (scaledImage == 0) {
        return debugMemError();
    }

    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            int _i = (int)((float)(i) / scaleY);
            int _j = (int)((float)(j) / scaleX);
            scaledImage[i * new_width + j] = buffer->pixels[_i * buffer->wSize + _j];
        }
    }

    free(buffer->pixels);

    buffer->pixels = scaledImage;
    buffer->wSize = new_width;
    buffer->hSize = new_height;
}
