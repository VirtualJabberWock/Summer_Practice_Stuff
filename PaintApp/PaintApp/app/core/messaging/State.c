#include "State.h"
#include "../DebugUtil.h"
#pragma once

State* NewState()
{
	State* state = calloc(1, sizeof(State));
	if (state == 0) {
		return debugMemError();
	}
	state->current = 0;
	state->__next__ = 0;
	return state;
}

void StateSet(State* state, objectVirtualFunc id, StatePriority p)
{
	if (p = ST_PRIORITY_HIGH) {
		StateClear(state);
	}
	if (state->current == 0 && state->__next__ == 0) {
		state->current = id;
		return;
	}
	if (state->current == 0 && state->__next__ != 0) {
		state->current = state->__next__;
		state->__next__ = id;
		return;
	}
	state->current = state->__next__;
	state->__next__ = id;
}

void StateClear(State* state)
{
	state->__next__ = 0;
	state->current = 0;
}

void StateNotify(State* state)
{
	if (state->current != 0) {
		if (isExecuteable(state->current)) {
			state->current(state);
		}
		state->current = state->__next__;
		state->__next__ = 0;
	}
}
