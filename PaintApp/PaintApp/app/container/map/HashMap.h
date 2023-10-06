#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "..\..\core\DebugUtil.h"
#include "..\..\core\Object.h"
#include "..\Map.h"
#include "..\List.h"
#include "..\ObjectVector.h"

typedef ObjectVector ListVector;

typedef struct tagHashMap {

	EXTENDS_MAP;

	ListVector* data;
	__int64 static_size;

} HashMap;


HashMap* NewHashMap();
HashMap* NewHashMapEx(int optSize);
void FreeHashMap(HashMap* hashmap);