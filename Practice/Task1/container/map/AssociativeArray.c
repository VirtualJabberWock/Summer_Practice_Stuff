#include "AssociativeArray.h"
#include "..\..\core\String.h"

DEFINE_MAP_TYPE(ArrayMap);

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

ArrayMap* NewArrayMap(int capacity)
{
	if (capacity <= 0) capacity = 5;
	ArrayMap* map = calloc(1, sizeof(ArrayMap));
	if (map == 0) {
		return 0;
	}
	MAP_SUPER(ArrayMap, map);
	OverrideIMapEmplace(ArrayMap, emplace);
	OverrideIMapGet(ArrayMap, get);
	OverrideObjectDispose(ArrayMap, FreeArrayMap);
	map->keys = calloc(capacity, sizeof(Object*));
	map->values = calloc(capacity, sizeof(Object*));
	map->size = 0;
	map->capacity = capacity;
	return map;
}

void FreeArrayMap(ArrayMap* map)
{
	if (map == 0) {
		return;
	}
	for (int i = 0; i < map->size; i++) {
		DestroyObject(&map->keys[i]);
		DestroyObject(&map->values[i]);
	}
	free(map->keys);
	free(map->values);
	map->keys = 0;
	map->values = 0;
	map->capacity = 0;
	map->size = 0;
}
