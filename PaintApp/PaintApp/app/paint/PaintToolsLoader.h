#pragma once

#include "../core/Object.h"
#include "../container/map/HashMap.h"
#include "../container/Map.h"
#include "PaintTools.h"

typedef struct tagPaintToolsLoader {

	IMap* loadedTools;

} PaintToolsLoader;

void PaintToolsLoaderInit();

PT_EXPORT PaintToolsLoader* GetPaintToolsLoaderInstance();

PT_EXPORT IPaintTool* GetLoadedPaintTool(const char* name);

PT_EXPORT int RegisterCustomPaintTool(IPaintTool* customTool);

char isPaintToolIsStub(IPaintTool* tool);