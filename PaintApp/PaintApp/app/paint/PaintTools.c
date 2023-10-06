#include "PaintTools.h"
#include "../win/SimpleGraphics.h"

DEFINE_OBJ_TYPE(IPaintTool);

void InitPaintToolsResourses()
{

}

void UpdatePaintToolProperties(PaintContext* tool, int width, UINT color, char isFill)
{
	if (tool->pen != 0) DeleteObject(tool->pen);
	if (tool->brush != 0) DeleteObject(tool->brush);
	tool->width = width;
	tool->color = color;
	tool->pen = CreatePen(PS_SOLID, width, color);
	tool->brush = CreateSolidBrush(color);
	tool->isFill = isFill;
}

IPaintTool* CreateIPaintTool(PT_DrawFunc drawFunc, PT_DrawFunc drawIconFunc, const char* name) {
	IPaintTool* tool = calloc(1, sizeof(IPaintTool));
	if (tool == 0) {
		return debugMemError();
	}
	OBJECT_SUPER(IPaintTool, tool);
	tool->onDraw = drawFunc;
	tool->onDrawIcon = drawIconFunc;
	tool->onSessionCompleteEvent = doNothing;
	tool->onSessionStartEvent = doNothing;
	tool->onThemeUpdated = doNothing;
	tool->name = name;
	return tool;
}

PT_EXPORT IPaintTool* CreateIPaintToolEx(
	PT_DrawFunc drawFunc, PT_DrawFunc drawIconFunc,
	PT_EventFunc onSessionStart, PT_EventFunc onSessionComplete,
	const char* name
)
{
	IPaintTool* tool = CreateIPaintTool(drawFunc, drawIconFunc, name);
	tool->onSessionCompleteEvent = onSessionComplete;
	tool->onSessionStartEvent = onSessionStart;
	return tool;
}

IPaintTool* CastToIPaintTool(Object* obj)
{
	if (obj->__addr != (pointer_value) obj) {
		return 0;
	}
	if (obj->type != TYPE_IPaintTool) {
		return 0;
	}
	return obj;
}

#include "../win_classes/CanvasWindow.h"

PT_EXPORT ImageBitmap* GetPaintContextImageBitmap(PaintContext* context)
{
	CanvasWindow* ref = context->canvasReference;
	return ref->mainImage;
}




