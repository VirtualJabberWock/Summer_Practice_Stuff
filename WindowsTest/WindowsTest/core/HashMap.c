#pragma once
#include "HashMap.h"
#include "List.h"

DEFINE_TYPE(HashMap)

ObjectNode* HashMap_emplace_by_hash(struct tagHashMap* self, __int64 h, Object* value) {

	if (self == 0) throw NULL_POINTER_EXCEPTION;
	
	__int64 index = abs(h % self->static_size);
	
	if (index > self->data->size) {
		for (int i = 0; i < self->static_size - self->data->size; i++) {
			self->data->_->push(self->data, NULL);
		}
	}

	if (self->data->ptr[index] == NULL) {
		self->data->ptr[index] = NewObjectList();
	}

	ObjectList* l = (ObjectList*)(self->data->ptr[index]);
	ObjectNode* node = l->_->push(l, value);
	node->hash = h;
	return node;
}

void HashMap_emplace(struct tagHashMap* self, Object* key, Object* value) {

	if (self == 0) throw NULL_POINTER_EXCEPTION;
	if (key == 0) throw NULL_POINTER_EXCEPTION;

	if (isObject(key) == 0) {
		return 0; // TODO;
	}

	__int64 h = hashObject(key);

	if (key->type == String_TYPE && self->save_keys == 0) {
		if (CastString(key)->isInstantFree) {
			key->free(key);
			free(key);
		}
	}
	
	ObjectNode* node = HashMap_emplace_by_hash(self, h, value);
	if (self->save_keys) node->metadata = key;
}

void HashMap_emplace_by_str(struct tagHashMap* self, const char* key, Object* value) {

	if (self == 0) throw NULL_POINTER_EXCEPTION;
	if (key == 0) throw NULL_POINTER_EXCEPTION;

	__int64 h = Hash_C_String(key);
	ObjectNode* node = HashMap_emplace_by_hash(self, h, value);
	if (self->save_keys) node->metadata = NewString(key);
}

Object* HashMap_getByHash(struct tagHashMap* self, __int64 h) {

	if (self == NULL) throw NULL_POINTER_EXCEPTION;

	__int64 index = abs(h % self->static_size);
	if (index > self->static_size) 
		return panic("HASHMAP", 1); //TODO throw
	
	if (self->data->ptr[index] == NULL)
		return panic("Key not contains in HashMap (not exists)", ERR_KEY_DONT_EXISTS);

	ObjectList* l = self->data->ptr[index];
	if (checkObjectType(l, ObjectList_TYPE) == 0)
		throw INVALID_CAST_EXCEPTION("can't cast to ObjectList (HashMap element)");

	ObjectNode* current = l->head;
	if(l->head == NULL)
		return panic("Key not contains in HashMap (or deleted)", ERR_KEY_DONT_EXISTS);

	while (current != NULL)
	{
		if (current->hash == h) {
			return current->obj;
		}
		current = current->next;
	}

	return panic("Key not contains in HashMap (not found)", ERR_KEY_DONT_EXISTS);
}

Object* HashMap_get(struct tagHashMap* self, Object* key) {

	if (key == NULL) throw NULL_POINTER_EXCEPTION;

	if (isObject(key) == 0) return panic("HASHMAP : INVALID KEY", 2);

	__int64 h = hashObject(key);

	if (key->type == String_TYPE) {
		if (CastString(key)->isInstantFree) {
			key->free(key);
			free(key);
		}
	}

	return HashMap_getByHash(self, h);
}

Object* HashMap_gets(struct tagHashMap* self, const char* key) {

	if (key == NULL) throw NULL_POINTER_EXCEPTION;

	__int64 h = Hash_C_String(key);

	return HashMap_getByHash(self, h);
}


struct HashMap_mtable_tag HashMap_METHODS[] = {
	{ 
		HashMap_emplace,
		HashMap_emplace_by_str,
		HashMap_emplace_by_hash ,
		HashMap_get, 
		HashMap_gets,
		HashMap_getByHash 
	} 
};

#define DEFAULT_SIZE 67

HashMap* NewHashMap()
{
	HashMap* map = (HashMap*)malloc(sizeof(HashMap));
	if (map == 0) MEM_PANIC_RETURN_0;
	
	OBJECT_SUPER_FM(HashMap, map);

	map->free = FreeHashMap;

	map->data = NewObjectVector(67);
	map->static_size = DEFAULT_SIZE;
	map->save_keys = 0;
	map->keys = 0;

	for (int i = 0; i < map->static_size; i++) {
		map->data->_->push(map->data, NULL);
	}

	return map;

}

Dictionary* NewDictionary()
{
	Dictionary* dict = (Dictionary*) NewHashMap();
	dict->save_keys = 1;
	return dict;
}

void FreeHashMap(HashMap* hashmap)
{
	if (checkObjectType(hashmap, HashMap_TYPE) == 0) {
		return;
	}
	hashmap->static_size = 67;
	hashmap->data->free(hashmap);
	free(hashmap->data);
	hashmap->data = 0;
}
