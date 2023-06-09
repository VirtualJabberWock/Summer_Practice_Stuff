#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "..\..\core\DebugUtil.h"
#include "..\..\core\Object.h"
#include "..\Map.h"
#include "..\List.h"
#include "..\ObjectVector.h"

typedef struct tagHashMap {

	EXTENDS_MAP;

	ObjectVector* data;
	__int64 static_size;

} HashMap;


HashMap* NewHashMap();
void  FreeHashMap(HashMap* hashmap);