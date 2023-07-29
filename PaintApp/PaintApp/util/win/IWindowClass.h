#pragma once

#include "..\core\Object.h"

#include <Windows.h>

#include "..\win\Context.h"

typedef struct tagWindowClassVTABLE {

	ATOM (*registerClass)(Object* windowClass, HINSTANCE hInstance);
	HWND (*create)(Object* windowClass, WindowContext* optParent);
	LRESULT (*OnMessage)(Object* windowClass, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

} WindowClassVTABLE;

typedef struct  tagWindowClass {

	EXTENDS_OBJECT;

	WindowClassVTABLE* methods;
	WindowContext context;
	char* winClassName;
	char isRegistred;

} IWindowClass;

void IWindowRegister(IWindowClass* window, HINSTANCE hInstance);
BOOL IWindowCreateAndShow(IWindowClass* window, WindowContext* optParent);
LRESULT CALLBACK IWindowSendMessage(IWindowClass* window, UINT message, WPARAM wParam, LPARAM lParam);
void IWindowInvalidate(IWindowClass* window, RECT* optionalRegion);
int IWindowClose(IWindowClass* window);

void InitWindowClass(IWindowClass* map, char* className, objectInternalMethod* mapMethods, objectInternalMethod* objectMethods);

HINSTANCE IWindowGetHINSTANCE(IWindowClass* window);

#define EXTENDS_WINDOWCLASS IWindowClass __wndClass

#define WINDOWCLASS_SUPER(type, ptr) \
InitWindowClass(ptr, "MYWIN_"###type, IWindowClass_##type##_InternalMethods, Object_##type##_InternalMethods);

LRESULT CALLBACK IWindowClass_defaultMessageHandler(IWindowClass* window, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#define DEFINE_WINDOWCLASS_TYPE(type) \
__declspec(dllexport) const char* type##_TYPE(){return #type;}; \
static objectInternalMethod Object_##type##_InternalMethods[4] = { \
	ObjectDefaultDispose, ObjectDefaultHash, ObjectDefaultEquals, ObjectDefaultCompare \
}; \
static objectInternalMethod IWindowClass_##type##_InternalMethods[3] = { \
	doNothing, doNothing, IWindowClass_defaultMessageHandler \
};

#define OverrideWindowClassRegister(type, func) \
IWindowClass_##type##_InternalMethods[0] = func;

#define OverrideWindowClassCreate(type, func) \
IWindowClass_##type##_InternalMethods[1] = func;

#define OverrideWindowClassOnMessage(type, func) \
IWindowClass_##type##_InternalMethods[2] = func;