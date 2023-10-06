#include "BrushTool.h"
#include "../../win/SimpleGraphics.h"

static HPEN toolIconRectEdge = 0;

static POINT* points;
static int p_size;
static int last_x;
static int last_y;

static int max_x;
static int max_y;
static int min_x;
static int min_y;

void RenderTool(RECT* frame, PaintContext* props, HDC hdc, HWND hWnd)
{
	int dx = abs(last_x - frame->right);
	int dy = abs(last_y - frame->bottom);
	if (dx > 0 && dy > 0 && p_size < 998) {
		last_x = frame->right;
		last_y = frame->bottom;
		points[p_size].x = last_x;
		points[p_size].y = last_y;
		p_size++;
	}
	if (last_x > max_x) max_x = last_x;
	if (last_y > max_y) max_y = last_y;
	if (last_y < min_x) min_x = last_x;
	if (last_y < min_y) min_y = last_y;
	int xx = -1;
	int yy = -1;
	for (int i = 0; i < p_size; i++) 
	{
		if (xx != -1 && yy != -1)
			DrawLine(hdc, xx, yy, points[i].x, points[i].y, props->pen);
		xx = points[i].x;
		yy = points[i].y;
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
	points = calloc(1000, sizeof(POINT));
	p_size = 0;
}

static void OnComplete(RECT* region, PaintContext* context, HDC hdc, HWND hWnd)
{
	p_size = 0;
	free(points);
	region->left = min_x;
	region->top = min_y;
	region->right = max_x;
	region->bottom = max_y;
}

IPaintTool* CreateBrushTool()
{
	return CreateIPaintToolEx(RenderTool, RenderIcon, OnStart, OnComplete, "BrushTool");
}
