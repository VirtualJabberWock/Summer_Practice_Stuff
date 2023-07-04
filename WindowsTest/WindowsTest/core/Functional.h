#pragma once
#include "Object.h"

typedef union _tag_AnyValueContainer {
	void* ptr;
#if _WIN32 || _WIN64
#if _WIN64
	__int64 number;
	double d;
	char bytes[8];
	__int32 HL[2]; // HIGH LOW
	
#else
	__int32 number;
	float d;
	char bytes[4];
	__int16 HL[2]; // HIGH LOW
#endif
#endif
} AnyValueContainer;

typedef void (*OnResponse)(int status, const char* data);
typedef void (*OnAction)();
typedef void (*VoidWrap) (void* arg0, ...);
typedef void* (*AnyFunction) (void* arg0, ...);
typedef __int64 (*NoArgs64) ();
typedef __int32 (*NoArgs32) ();

typedef __int64 (*func_prototype64) (void* arg0, ...);
typedef __int32 (*func_prototype86) (void* arg0, ...);

__int64 DoNothing(void* arg0, ...);
void NotImplemented(void* arg0, ...);

#if _WIN32 || _WIN64
#if _WIN64
#define func_prototype func_prototype64
#define NoArgsFunction NoArgs64
#else
#define func_prototype func_prototype86
#define NoArgsFunction NoArgs32
#endif
#endif

typedef struct tagAnonymousContext {
	char type;
	void* _raw_data; // context;
} AnonymousContext;

typedef struct tagNativeAnonymousContext {
	char type;
	AnyValueContainer _[16]; // context;
} NativeAnonymousContext;

typedef struct tagMappedAnonymousContext {
	char type;
	Object* hashmap; // context;
} MappedAnonymousContext;

typedef struct tagSingleAnonymousContext {
	char type;
	Object* obj; // context;
} SingleAnonymousContext;


NativeAnonymousContext* Functional_getNativeContext(AnonymousContext* context);
/*@return hashmap*/
Object* Functional_getMappedContext(AnonymousContext* context);
SingleAnonymousContext* Functional_getSingleObjectContext(AnonymousContext* context);


typedef struct tagFuncContainer {
	func_prototype func;
} FuncContainer;

typedef void CanThrowAnException;
