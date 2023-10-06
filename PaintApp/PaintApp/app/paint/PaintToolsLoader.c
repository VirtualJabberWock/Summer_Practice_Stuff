#include "PaintToolsLoader.h"

#include "../core/String.h"

#include "tools/RectTool.h"
#include "tools/LineTool.h"
#include "tools/ColorPickerTool.h"
#include "tools/BrushTool.h"

static PaintToolsLoader* instance = 0;
static IPaintTool* selectToolStub = 0;

void PaintToolsLoaderInit()
{
	PaintToolsLoader* ref = GetPaintToolsLoaderInstance();

	selectToolStub = CreateIPaintTool(doNothing, doNothing, "SelectTool");

	mapEmplace(ref->loadedTools, NewTempString("SelectTool"), selectToolStub);
	mapEmplace(ref->loadedTools, NewTempString("RectTool"), CreateRectTool());
	mapEmplace(ref->loadedTools, NewTempString("LineTool"), CreateLineTool());
	mapEmplace(ref->loadedTools, NewTempString("ColorPickerTool"), CreateColorPickerTool());
	mapEmplace(ref->loadedTools, NewTempString("BrushTool"), CreateBrushTool());
}

PaintToolsLoader* GetPaintToolsLoaderInstance()
{
	if (instance == 0) {
		instance = calloc(1, sizeof(PaintToolsLoader));
		if (instance == 0) {
			return debugMemError();
		}
		instance->loadedTools = NewHashMap();
	}

	return instance;
}

PT_EXPORT IPaintTool* GetLoadedPaintTool(const char* name)
{
	PaintToolsLoader* ref = GetPaintToolsLoaderInstance();
	return mapGetByString(ref->loadedTools, name);
}

PT_EXPORT int RegisterCustomPaintTool(IPaintTool* customTool)
{
	if (CastToIPaintTool(customTool) == 0) {
		return 0;
	}
	PaintToolsLoader* ref = GetPaintToolsLoaderInstance();
	IPaintTool* tool = mapGetByString(ref->loadedTools, customTool->name);
	if (tool == 0) {
		mapEmplace(ref->loadedTools, NewString(customTool->name), customTool);
		return 1;
	}
	return 1;
}

char isPaintToolIsStub(IPaintTool* tool)
{
	return (tool == selectToolStub);
}
