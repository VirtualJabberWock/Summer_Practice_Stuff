#pragma once

#define __OBJECT_HEADER 1337

/*

To create a full working Object and ObjectClass,

using OBJECT_CLASS_FM (fields and methods) or OBJECT_CLASS_F (fields)

You need:

1. Create object class, like this:

OBJECT_CLASS_FM(typename,
						         // fields:
		char field_1;
		int field_2;
		__int64 field_3;
	,                            // methods: (after comma)
		void (*method_1)(int);
		void (*method_2)(int, int);
		int (*method_3)();
)

2. Define the object type in '.c' file by macro:

	DEFINE_TYPE(typename);

3. When you creating an Object (like SomeClass* NewSomeClass() ), don't forget call OBJECT_SUPER:

	...
	SomeClass* ptr = malloc(sizeof(SomeClass))
	if(ptr == NULL) return 0;
	OBJECT_SUPER_FM(SomeClass, ptr); <---
	...

4. If you have methods in ''object class'',
you should init method_table in '.c' file like this:

	struct SomeClass_mtable_tag SomeClass_METHODS[] = {{ <funtions from '.c' file> }};

#You can look for exapmples in ./CommonTypes.h

*/

typedef void* _methods_table;

/*
About Object:

__int16 header -- some magic number to check if struct is Object type;
__int64 type -- some autogenerated (by macro) id of object type (ex. String_TYPE = 0xe312324213215)
void (*free) -- [virtual] if your Object type have mem allocs, you can implement your own free func;
void (*compare) -- [virtual] if your Object type can be compared, you can implement your own compare func;
_methods_table -- [Stub!] some pointer to methods ( Call: obj->_->method(...) )

*/
typedef struct tagObject {

	__int16 header;
	__int64 type; 
	void (*free) (struct tagObject* obj); 
	/*@return 0 if equals, 1 if self > other, -1 self < other */
	int (*compare) (struct tagObject* self, struct tagObject* other, __int64* opt_OutHash);
	_methods_table _method_table;

} Object;

typedef char* (*ObjectType)(); //

static const __int64 Object_ANYTYPE = 0;

/* very bad, but it works */

#define DEFINE_TYPE(name) char* name##_TYPE(){ return #name; };

#define OBJECT_CLASS_FM(name, fields, methods) typedef struct name##_mtable_tag { methods } name##_mtable; \
__declspec(dllexport) char* name##_TYPE(); \
\
typedef struct tag##name { \
\
	__int16 __header; \
	__int64 __type; \
	void (*free) (struct tag##name* with); \
    int (*compare) (struct tag##name* self, struct tag##name* other, __int64* opt_OutHash);\
	name##_mtable* _; \
	fields \
\
} name; // END OF MACROS

#define OBJECT_CLASS_F(name, fields, methods) __declspec(dllexport) char* name##_TYPE(); \
typedef struct tag##name { \
	__int16 __header;\
	__int64 __type; \
	void (*free) (struct tag##name* with); \
    int (*compare) (struct tag##name* self, struct tag##name* other, __int64* opt_OutHash);\
	_methods_table __not_implemented__; \
	fields \
} name; // END OF MACROS

int isObject(Object* s);
int checkObjectType(Object* s, ObjectType type);
#define OBJECT_SUPER_F(name, ptr) \
 ptr->__header = 1337; \
 ptr->__type = name##_TYPE; \
 ptr->free = standartFree; \
 ptr->compare = standartCompare; \

#define OBJECT_SUPER_FM(name, ptr) \
 ptr->__header = 1337; \
 ptr->__type = name##_TYPE; \
 ptr->free = standartFree;\
 ptr->compare = standartCompare; \
 ptr->_ = name##_METHODS;


#define CHECK_OBJ_TYPE(with, name) checkObjectType(with, name##_TYPE)
#define DISPOSE_OBJECT(obj) {if(obj != 0){if(obj->free != 0){obj->free(obj);} free(obj); obj = 0;}}

void standartFree(Object* obj);

/*
if self > with return 1;
if self < with  return -1;
if self == with  return 0;
Also it write hash of [self] to opt_hashOut;
*/
int standartCompare(Object* self, Object* with, __int64* opt_hashOut);

int isObjectEquals(Object* obj1, Object* obj2);
int CompareObjects(Object* base, Object* compareWith);
__int64 hashObject(Object* obj);

