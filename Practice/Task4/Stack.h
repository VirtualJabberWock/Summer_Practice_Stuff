#pragma once

#define MAX_STACK_SIZE 255

typedef struct tagStack {

	int topIndex;
	int elemSize;
	void* data;

} Stack;

typedef struct tagCharStack {

	int topIndex;
	int elemSize;
	char* data;

} CharStack;

CharStack* NewCharStack();

void pushToStack(Stack* stack, __int64 value);
__int64 getStackTop(Stack* stack);

