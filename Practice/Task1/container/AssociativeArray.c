#include "AssociativeArray.h"
#include "..\core\String.h"

static void emplace(ArrayMap* map, Object* key, Object* value)
{

	if (map->size + 1 > map->capacity) {
		map->capacity *= 2;
		map->keys = realloc(map->keys, map->capacity * sizeof(Object*));
		map->values = realloc(map->values, map->capacity * sizeof(Object*));
	}
	
	map->keys[map->size] = key;
	map->values[map->size] = value;

	map->size++;
}

static Object* get(ArrayMap* map, Object* key) {
	
	for (int i = 0; i < map->size; i++) {
		if (compareObjects(map->keys[i], key) == 0) {
			return map->values[i];
		}
	}

	return 0;
}

static Object* getByStr(ArrayMap* map, const char* str)
{
	String* temp = NewString(str);
	Object* obj = get(map, temp);
	DestroyObject(&temp);
	return obj;
}

ArrayMapVTABLE* ARRAY_MAP_VTABLE[3] = {
	emplace, get, getByStr
};

ArrayMap* NewArrayMap(int capacity)
{
	if (capacity <= 0) capacity = 5;
	ArrayMap* map = calloc(1, sizeof(ArrayMap));
	map->keys = calloc(capacity, sizeof(Object*));
	map->values = calloc(capacity, sizeof(Object*));
	map->size = 0;
	map->_ = ARRAY_MAP_VTABLE;
	map->capacity = capacity;
	return map;
}
