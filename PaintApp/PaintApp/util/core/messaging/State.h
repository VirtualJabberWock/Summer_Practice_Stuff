#pragma once

#include "..\Object.h"

typedef struct tagState {

	objectVirtualFunc current;
	objectVirtualFunc __next__;

} State;

typedef enum enumStatePriority {

	ST_PRIORITY_NONE = 0,
	ST_PRIORITY_HIGH = 1,

} StatePriority;

State* NewState();
void StateSet(State* state, objectVirtualFunc handle, StatePriority p);
void StateClear(State* state);
void StateNotify(State* state);