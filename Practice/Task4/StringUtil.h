#pragma once
#include "Annotations.h"
#include <stdlib.h>

NEW char** splitByChar(char* str, char delimiter, int* retSize);

NEW char* CopyString(char* source, OUT int* retSize);

char** consoleArgumentsEmulation(IN char* raw_command_line, OUT int* outArgCount);