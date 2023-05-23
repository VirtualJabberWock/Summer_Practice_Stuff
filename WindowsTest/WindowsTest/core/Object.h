#pragma once

#define __OBJECT_HEADER 1337

/*

To create a full working Object and ObjectClass,

using OBJECT_CLASS_FM (fields and methods) or OBJECT_CLASS_F (fields)

You need:

1. Create object class, like this:

OBJECT_CLASS_FM(name,

		char field_1;
		int field_2;
		__int64 field_3;
	,
		void (*method_1)(int);
		void (*method_2)(int, int);
		int (*method_3)();
)

2. When you creating an Object, don't forget call OBJECT_SUPER:

	...
	SomeClass* ptr = malloc(sizeof(SomeClass))
	if(ptr == NULL) return 0;
	OBJECT_SUPER_FM(SomeClass, ptr); <---
	...

3. If you have methods in ''object class'',
you should init method_table in '.c' file like this:

	struct SomeClass_mtable_tag SomeClass_METHODS[] = {{ <funtions from '.c' file> }};

*/

typedef void* _methods_table;

typedef struct tagObject {

	__int16 header; // 2,  to check is Object* isCorrupted
	__int64 type; // 8
	void (*free) (struct tagObject* obj); // 8
	_methods_table _mt;

} Object; // 18 bytes

typedef void (*ObjectType)(); // pointer to char

/* very bad, but it works */

#define OBJECT_CLASS_FM(name, fields, methods) typedef struct name##_mtable_tag { methods } name##_mtable; \
static char* name##_TYPE(){ return #name; };\
\
typedef struct tag##name { \
\
	__int16 header; \
	__int64 type; \
	void (*free) (struct tag##name* obj); \
	name##_mtable* _mt; \
	fields \
\
} name;

#define OBJECT_CLASS_F(name, fields, methods) static char* name##_TYPE(){return #name;}; \
typedef struct tag##name { \
	__int16 header;__int64 type; \
	void (*free) (struct tag##name* obj); \
	_methods_table _mt; \
	fields \
} name;

int isObject(Object* s);
int checkObjectType(Object* s, ObjectType type);
#define OBJECT_SUPER_F(name, ptr) ptr->header = 1337; ptr->type = name##_TYPE; ptr->free = 0; ptr->_mt = 0;
#define OBJECT_SUPER_FM(name, ptr) ptr->header = 1337; ptr->type = name##_TYPE; ptr->free = 0; ptr->_mt = name##_METHODS;


#define CHECK_OBJ_TYPE(obj, name) checkObjectType(obj, name##_TYPE)



