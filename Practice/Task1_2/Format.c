#include "Format.h"

#include <stdlib.h>
#include <stdio.h>

static const char* leadingZeros = "0000000000000000\0                ";
static const char* leadingDots =  "........\0HIDDEN!         ";

/*

ƒополнительно поддерживать опцию
-f format_str	указание форматной строки, определ€ющей формат вывода каждой строки.
‘орматна€ строка состоит из обычных символов, спецсимволов и спецификаторов.
ќбычными символами считаютс€ все символы, которые не €вл€ютс€ спецсимволами или частью спецификатора.
—пецсимволами €вл€ютс€ символы '\' и '%', которые мен€ют интерпретацию следующего за ними символа. ѕоддерживать следующие последовательности:
"\n" - байт перевода строки (0x0A).
"\r" - байт возврата каретки (0x0D).
"\t" - байт табул€ции (0x09).
"\\" - символ '\'.
—пецификаторы соответствуют выводимым кусочкам (их количество в строке и размер задаЄтс€ параметрами -n, -g) файла.
—пецификаторы имеют вид:
%i - индекс текущей строки
%n - смещение текущей строки
%ix - вывод i-го кусочка в шестнадцатиричном виде
%ic - вывод i-го кусочка в символьном виде (1, 2 или 4 символа в зависимости от размера кусочка)

*/

//"%i %n %0x %1x %2x %3x %4x %5x %6x %7x %8x %9x %10x %11x %12x %13x %14x %15x | %0c%1c%2c%3c%4c%5c%6c%7c%8c%9c%10c%11c%12c%13c%14c%15c\n"

int isFormatValid(char* format, int* outSize, ByteDataGroup chunkType)
{
	char token = 0;
	int i = 0;
	while (format[i] != 0) {
		if (format[i] != '%') { i++;  continue; }
		else {
			token = format[i + 1];
			if (token == 0) return FORMAT_ERR_INVALID_TOKEN;
			if (token == ' ') return FORMAT_ERR_INVALID_TOKEN;
			if (token == 'n') { i++;  continue; }
			if (token == 'i') { i++;  continue; }
			if (token >= '0' && token <= '9'){
				int j = 0;
				while (1) {
					if (format[i + 2 + j] == 0) return FORMAT_ERR_INVALID_TOKEN;
					if (format[i + 2 + j] == 'x') { break; }
					if (format[i + 2 + j] == 'c') { 
						break; 
					}
					if (format[i + 2 + j] == 'с') { 
						printf("\n\nError [Format Flag]: Russian 'c', do you mean 'c' in latin?\n\n");
						return FORMAT_ERR_INVALID_TOKEN;
					}
					if (format[i + 2 + j] == 'c') { 
						break; 
					}
					if (format[i + 2 + j] < '0' || format[i + 2 + j] > '9')
						return FORMAT_ERR_INVALID_TOKEN;
					j++;
				}
				{ i++;  continue; }
			}
			printf("\n\nError [FormatFlag]: Wrong token [%c] after '%c' \n\n", token, '%');
			return FORMAT_ERR_INVALID_TOKEN;
		}
		i++;
	}
	if (outSize != 0) *outSize = i;
	return 0;
}

static int readNumber(char* from, char* outEndChar, int* outSkip) {
	int i = 0;
	while (from[i] >= '0' && from[i] <= '9') {
		i++;
	}
	char tmp = from[i];
	from[i] = 0;
	int number = atoi(from);
	from[i] = tmp;
	if (outEndChar != 0) *outEndChar = tmp;
	if(outSkip != 0) *outSkip = i;
	return number;
}

static inline int printChunk(char* buffer, int index, ByteDataGroup chunkType) {
	if (chunkType == BYTE)  return printf("%02hhX",  ((   char*)(buffer))[index]);
	if (chunkType == WORD)  return printf("%04hX",    ((__int16*)(buffer))[index]);
	if (chunkType == DWORD) return printf("%08X",    ((__int32*)(buffer))[index]);
	if (chunkType == QWORD) return printf("%016llX", ((__int64*)(buffer))[index]);
}

static inline int printChunkChar(char* buffer, int index, ByteDataGroup chunkType) {
	for (int i = index; i < index + chunkType; i++) {
		unsigned char t = buffer[i];
		if(isprint(t)) fwrite(buffer + i, sizeof(char), 1, stdout);
		else fwrite(".", sizeof(char), 1, stdout);
	}
}

void printFormatString(
	int lineIndex, int currentOffset,
	char* byteBuffer, int bufferSize,
	DisplayOptions* opt
)
{
	char token = 0;
	char* format = opt->format; // reference
	int i = 0;

	int printerMode = (opt->chunkType == BYTE) * 16;

	int tokenOverflowLimit = INT_MAX / opt->chunkType;

	//printf("\n Debug: bufferSize = %d, chunkType = %d\n", bufferSize, opt->chunkType);

	while(i < opt->formatSize) {
		if (format[i] == 0) break;
		char seek = format[i];
		if (seek != '%' && seek != '\\') {
			fwrite(format + i, 1, sizeof(char), stdout);
			i++;
			continue;
		}
		else if(seek != '\\') {
			token = format[i + 1];
			if (token == 0) return;
			if (token == 'i') printf("%d", lineIndex);
			if (token == 'n') printf("%08x", currentOffset);
			if (token >= '0' && token <= '9') {
				char type = 0;
				int skip = 0;
				int number = readNumber(format + i + 1, &type, &skip);
				if (number >= tokenOverflowLimit || number < 0) 
					number = (tokenOverflowLimit - opt->chunkType);
				i += skip;
				int isInvalidByte = (number * opt->chunkType >= bufferSize);
				if (type == 'x') {
					if (isInvalidByte)
						fwrite(leadingZeros+printerMode, 2, opt->chunkType, stdout);
					else 
						printChunk(byteBuffer, number, opt->chunkType);
				}
				if (type == 'c') {
					if (isInvalidByte)
						fwrite(leadingDots+printerMode, 1, opt->chunkType, stdout);
					else 
						printChunkChar(byteBuffer, number*opt->chunkType, opt->chunkType);
				}
			}
			i++;
		}
		else {
			token = format[i + 1];
			if (token == 'n') printf("\n");
			if (token == 'r') printf("\r");
			if (token == '\\') printf("\\");
			if (token == 't') printf("\t");
			i++;
		}
		i++;
	}
}


