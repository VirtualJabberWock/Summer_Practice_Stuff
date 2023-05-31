#pragma once
#include "HashMap.h"

DEFINE_TYPE(HashMap)



void HashMap_emplace(struct tagHashMap* self, Object* key, Object* value) {

	if (self == 0) throw NULL_POINTER_EXCEPTION;
	if (key == 0) throw NULL_POINTER_EXCEPTION;
	
	if (isObject(key) == 0) {
		return 0; // TODO;
	}

	if (key->type != self->common_type && self->common_type != 0) 
		return 0;


}

Object* HashMap_get(struct tagHashMap* self, Object* key) {

}


struct HashMap_mtable_tag HashMap_METHODS[] = { { HashMap_emplace, HashMap_get } };


HashMap* NewHashMap(ObjectType common_type)
{
	HashMap* map = (HashMap*)malloc(sizeof(HashMap));
	if (map == 0) MEM_PANIC_RETURN_0;
	
	OBJECT_SUPER_FM(HashMap, map);



}
