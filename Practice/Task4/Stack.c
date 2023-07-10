#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>


CharStack* NewCharStack()
{
	CharStack* stack = calloc(1, sizeof(CharStack));
	if (stack == 0) return 0;
	stack->data = calloc(MAX_STACK_SIZE, sizeof(char));
	stack->topIndex = 0;
	stack->elemSize = sizeof(char);
	return stack;
}

void pushToStack(Stack* stack, __int64 value)
{
	int offset = stack->topIndex * stack->elemSize;
	char* a = (char*)stack->data + offset;
	memcpy(a, &value, stack->elemSize);
	stack->topIndex += 1;
	if (stack->topIndex > MAX_STACK_SIZE) {
		exit(printf("Stack Overflow!"));
	}
}

__int64 getStackTop(Stack* stack)
{
	__int64 value = 0;
	char* bytes = stack->data;
	memcpy(&value, bytes + stack->topIndex * stack->elemSize, stack->elemSize);
	return value;
}


