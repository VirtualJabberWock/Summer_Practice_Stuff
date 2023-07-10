#pragma once
#include "DisplayOptions.h"

#define FORMAT_ERR_INVALID_TOKEN 1

int isFormatValid(char* format, int* outSize, ByteDataGroup chunkType);
void printFormatString(int lineIndex, int currentOffset, char* byteBuffer, int bufferSize, DisplayOptions* opt);