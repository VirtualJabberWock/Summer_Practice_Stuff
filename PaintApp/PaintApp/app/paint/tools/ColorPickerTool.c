#include "LineTool.h"

#include "../../win/SimpleGraphics.h"
#include "../../image/ImageTransform.h"

static HPEN toolIconRectEdge = 0;
static UINT choosenPixel = 0;

static HBITMAP toolImageIconDef = 0;
static HBITMAP toolImageIconS = 0;
static HBITMAP toolImageIconP = 0;

static void ToolImageIterator(UINT* pixels, int width, int height, int yFrom, int yTo, int xFrom, int xTo) {
	UINT r = GET_RED(pixels[yFrom * width + xFrom]);
	UINT g = GET_GREEN(pixels[yFrom * width + xFrom]);
	UINT b = GET_BLUE(pixels[yFrom * width + xFrom]);
	choosenPixel = MAKE_COL(b, g, r);
	
}

static void OnStart(RECT* frame, PaintContext* context, HDC hdc, HWND hWnd) {
	ImageBitmap* image = GetPaintContextImageBitmap(context);
	TransformImageBuffer(image, ToolImageIterator, frame);
}

#include "../../core/messaging/EventBus.h"
#include "../../events/PaintToolsEvents.h"

static void OnComplete(RECT* region, PaintContext* context, HDC hdc, HWND hWnd)
{
	if (context->color == choosenPixel) return;
	context->color = choosenPixel;
	if (context->pen != 0) DeleteObject(context->pen);
	if (context->brush != 0) DeleteObject(context->brush);
	context->pen = CreatePen(PS_SOLID, context->width, context->color);
	context->brush = CreateSolidBrush(context->color);
	Event* onPropsUpdate = PT_NewPropertiesUpdateEvent();
	EventBus_postEvent(onPropsUpdate);
	DestroyObject(&onPropsUpdate);
}

#include "../../appearance/Theme.h"
#include "../../image/TextureUtil.h"

static void RenderIcon(RECT* rect, PaintContext* tool, HDC hdc, HWND hWnd, ToolButtonState state)
{
	if (toolImageIconDef == 0) return;
	if (state == TBUTTON_STATE_FOCUS) {
		DrawBitmap(hdc, toolImageIconS, rect->left + 25, rect->top + 9);
	}
	else if (state == TBUTTON_STATE_SELECTED) {
		DrawBitmap(hdc, toolImageIconP, rect->left + 25, rect->top + 9);
	}
	else {
		DrawBitmap(hdc, toolImageIconDef, rect->left + 25, rect->top + 9);
	}
}

static void OnThemeUpdated() 
{
	if (toolImageIconDef != 0) {
		DeleteObject(toolImageIconDef);
		DeleteObject(toolImageIconS);
		DeleteObject(toolImageIconP);
	}
	HBITMAP texture = LoadImageResource("color_picker_icon", GetCurrentTheme());
	toolImageIconDef = ReplaceAlphaChannel(texture, GetBrushColor(Theme_B_button), 0);
	toolImageIconS = ReplaceAlphaChannel(texture, GetBrushColor(Theme_B_buttonFocus), 0);
	toolImageIconP = ReplaceAlphaChannel(texture, GetBrushColor(Theme_B_buttonPressed), 0);
}

IPaintTool* CreateColorPickerTool()
{
	toolIconRectEdge = CreatePen(PS_SOLID, 2, 0);
	IPaintTool* tool = CreateIPaintToolEx(doNothing, RenderIcon, OnStart, OnComplete, "ColorPickerTool");
	tool->onThemeUpdated = OnThemeUpdated;
	return tool;
}
