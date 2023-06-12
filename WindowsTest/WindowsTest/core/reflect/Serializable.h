#pragma once

#include "..\CommonTypes.h"
#include "..\HashMap.h"
#include "..\Functional.h"

static inline const char* getString(HashMap* map, const char* name){
		return CastString(map->_->gets(map, name))->ptr;

}

typedef void* unknown_data;

OBJECT_CLASS_F(Method,
	const char* name;
	const char* signature;
	func_prototype opt_ptr; //Only if method is alive(runtime)
)

OBJECT_CLASS_F(Field,
	const char* name;
	const char* type;
	unknown_data opt_data; // Only if Field is alive(runtime)
)

OBJECT_CLASS_FM(ClassReflection,

	const char* typename;
	ObjectType opt_type_ptr; // runtime-only

	HashMap* methods;
	HashMap* fields;
,

	void (*ref) (struct tagClassReflection* self);

)


OBJECT_CLASS_FM(ObjectReflection,

	const char* typename;
	ObjectType opt_type_ptr;

	HashMap* methods;
	HashMap* fields;

	Object* opt_object; // runtime-only
,

	void (*ref) (struct tagObjectReflection* self);

);

#define RegisterMethod(sc, ptr) sc->methods->_->emplace_by_hash(sc->methods, Hash_C_String(__func__), writeMethodMacro(__FUNCSIG__, __func__, ptr));

static void Method_Free(Method* m) {
	if (m->name != 0) free(m->name);
	if (m->signature != 0) free(m->signature);
}

static Method* writeMethodMacro(const char* func_sig, const char* name, func_prototype ptr) {
	Method* method = calloc(1, sizeof(Method));
	OBJECT_SUPER_F(Method, method);
	if (method == 0) return;
	method->free = Method_Free;
	method->name = 0;
	method->opt_ptr = ptr;
	method->signature = 0;
	method->name = copy_c_string(name, 1000);
	method->signature = copy_c_string(func_sig, 1000);
	return method;
}

static ObjectReflection* MakeObjectRef(ClassReflection* ref, Object* obj_ptr) {
	ObjectReflection* obj_ref = calloc(1, sizeof(ObjectReflection));
	if (obj_ref == 0) throw MEM_PANIC_RETURN_0;
	OBJECT_SUPER_F(ObjectReflection, obj_ref); //todo methods
	
	obj_ref->_->ref = NotImplemented;
	obj_ref->fields = ref->fields;
	obj_ref->methods = ref->methods;
	obj_ref->typename = ref->typename;
	obj_ref->opt_type_ptr = ref->opt_type_ptr;
	return obj_ref;
}


