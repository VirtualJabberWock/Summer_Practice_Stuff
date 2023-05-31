#pragma once

#include "..\CommonTypes.h"
#include "..\HashMap.h"
#include "..\Functional.h"



static inline const char* getString(HashMap* map, const char* name){
		return CastString(map->_->gets(map, name))->ptr;
}

/*

Method:

name - name
signature - args and return value (like in JNI):
	if value is Object* and has type: Name_TYPE,
	for native types:
		I = int (int32)
		L = long long (int64)
		C = char (1 byte)
		F = float
		D = double
		S = short
		V = void
		O = Object* (raw)
	if it is pointer to struct: structName_S
	if it native pointer (array) it should have [ at start ([C - char array (char*));

	example:

	void func123(int a, int b, Vector* arg, Object* arg2, double* arr):
		name : func123
		sign : (I,I,Vector_TYPE,O,[D)V

	
*/

OBJECT_CLASS_F(Method,
	const char* name;
	const char* signature;
	func_prototype opt_ptr; //Only if method is alive(runtime)
)

OBJECT_CLASS_F(Field,
	const char* name;
	const char* type;
	void* opt_ptr; // Only if Field is alive(runtime)
)

OBJECT_CLASS_FM(ClassReflection,

	const char* typename;
	ObjectType opt_type_ptr; // runtime-only

	HashMap* methods;
	HashMap* fields;
,

	void (*ref) (struct tagClassReflection* self);

)