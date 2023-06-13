#pragma once
#include "NativeBridge.h"

int isNativeReflectionInitialized = 0;
NoArgsFunction getStackRBP = 0;


__int64 InitNativeReflection() {

	if (isNativeReflectionInitialized) return;

	unsigned char asm0[4] = { 0x48, 0x89, 0xe8, 0xc3 };
	getStackRBP = bakeCustomFunction(asm0, 4);
	isNativeReflectionInitialized = 1;
}