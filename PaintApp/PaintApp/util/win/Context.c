#include "Context.h"

void InitWindowContext(WindowContext* context, HWND hWnd, WindowContext* parent, HINSTANCE hInst)
{


	GetClientRect(hWnd, &context->rect);

	context->hWnd = hWnd;
	context->parent = parent;
	context->hInst = hInst;
	context->width = (size_t)(context->rect.right - context->rect.left);
	context->height = (size_t)(context->rect.bottom - context->rect.top);
}
