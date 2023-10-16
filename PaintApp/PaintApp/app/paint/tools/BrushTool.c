#include "BrushTool.h"
#include "../../win/SimpleGraphics.h"

static HPEN toolIconRectEdge = 0;
static HPEN tool_debug_pen = 0;
static HPEN tool_debug2_pen = 0;

static POINT* points;

int p_size;
int last_x;
int last_y;

int max_x = 0;
int max_y = 0;
int min_x = 0;
int min_y = 0;

int cursor = 0;

#include "../../events/CanvasEvents.h"
#include "../../core/messaging/EventBus.h"

void RenderTool(RECT* frame, PaintContext* props, HDC hdc, HWND hWnd)
{

	if (frame->right > max_x) max_x = frame->right;
	if (frame->bottom > max_y) max_y = frame->bottom;
	if (frame->right < min_x) min_x = frame->right;
	if (frame->bottom < min_y) min_y = frame->bottom;
	int dx = abs(last_x - frame->right);
	int dy = abs(last_y - frame->bottom);
	if (dx > props->width || dy > props->width) {
		last_x = frame->right;
		last_y = frame->bottom;
		points[p_size].x = last_x;
		points[p_size].y = last_y;
		if (p_size < 48) p_size++;
		else
		{
			for (int i = 1; i <= p_size; i++) {
				points[i - 1].x = points[i].x;
				points[i - 1].y = points[i].y;
			}
		}
		cursor++;
	}
	int xx = -1;
	int yy = -1;
	for (int i = p_size-1; i >= 0; i--)
	{
		if (xx != -1 && yy != -1)
			DrawLine(hdc, xx, yy, points[i].x, points[i].y, props->pen);
		xx = points[i].x;
		yy = points[i].y;
	}
	if (cursor % 25 == 0)
	{
		ApplyHDCEvent* e = NewApplyHDCEvent();
		e->x = min_x; e->y = min_y;
		e->width = max_x - min_x + props->width; e->height = max_y - min_y + props->width;
		EventBus_postEvent(e);
		free(e);
	}
}

static void RenderIcon(RECT* rect, PaintContext* tool, HDC hdc, HWND hWnd)
{
	DrawLine(
		hdc,
		rect->left + 12, rect->top + 10, rect->right - 12, rect->bottom - 10,
		toolIconRectEdge
	);
}

static void OnStart(RECT* frame, PaintContext* context, HDC hdc, HWND hWnd) 
{
	memcpy(&max_x, frame, sizeof(int) * 4);
	points = calloc(50, sizeof(POINT));
	p_size = 0;
	cursor = 0;
	tool_debug_pen = CreatePen(PS_SOLID, 1, 0x00ff00);
}

static void OnComplete(RECT* region, PaintContext* context, HDC hdc, HWND hWnd)
{
	p_size = 0;
	free(points);
	region->left = min_x-1;
	region->top = min_y;
	region->right = max_x+1;
	region->bottom = max_y;
	DeleteObject(tool_debug_pen);
}

IPaintTool* CreateBrushTool()
{
	return CreateIPaintToolEx(RenderTool, RenderIcon, OnStart, OnComplete, "BrushTool");
}
