#include "HexDisplay.h"
#include <stdlib.h>
#include "Format.h"

static void displayClassic(DisplayOptions* options) {

	if (options->chunkType > QWORD || options->chunkType < 0) return;
	if (options->chunksCount == -1) options->chunksCount = 16 / options->chunkType;
	char isASCIIShow = (options->chunkType == BYTE);
	int current = options->offset;
	int isReaded = 1;
	char* tempBuffer = 0;
	int bufferSize = options->chunksCount * options->chunkType;
	int bytesToRead = (options->bytesCount > 0) ? options->bytesCount : INT_MAX;
	fseek(options->file, current, SEEK_CUR);
	tempBuffer = calloc(bufferSize, sizeof(char));
	int invalidBytesCount = bufferSize - options->chunkType * 2;
	if (invalidBytesCount < 0) invalidBytesCount = 0;
	if (tempBuffer == 0) return;
	while (isReaded > 0) {
		int quote = max(min(bufferSize, bytesToRead), 0);
		int readedBytes = fread(tempBuffer, 1, quote, options->file);
		bytesToRead -= readedBytes;
		int readedChunks = readedBytes / options->chunkType;
		isReaded = (readedBytes > 0);
		if (isReaded == 0) break;
		printf("%08x  ", current);
		int i = 0;
		while (i < bufferSize) {
			if (i >= readedBytes) {
				for (int j = 0; j < options->chunkType; j++) printf("  ");
				printf(" ");
			}
			else {
				int j = i / options->chunkType;
				if (options->chunkType == BYTE)
					printf("%02hhx ", ((char*)(tempBuffer))[j]);
				if (options->chunkType == WORD)
					printf("%04x ", ((__int16*)(tempBuffer))[j]);
				if (options->chunkType == DWORD)
					printf("%08x ", ((__int32*)(tempBuffer))[j]);
				if (options->chunkType == QWORD)
					printf("%016llx ", ((__int64*)(tempBuffer))[j]);
				i += options->chunkType-1;
			}
			i++;
		}
		if (isASCIIShow) {
			printf(" | ");
			for (int k = 0; k < readedChunks; k++) {
				unsigned char t = tempBuffer[k];
				if (isprint(t))
					printf("%c", tempBuffer[k]);
				else printf(".");
			} 
		}
		for (int i = bufferSize - 1; i >= invalidBytesCount; i--) {
			tempBuffer[i] = 0;
		}
		current += options->chunksCount * options->chunkType;
		printf("\n");
	}

}

static void displayFormat(DisplayOptions* options) {

	if (options->chunkType > QWORD || options->chunkType < 0) return;
	if (options->chunksCount == -1) options->chunksCount = 16 / options->chunkType;
	char isASCIIShow = (options->chunkType == BYTE);
	int current = options->offset;
	int isReaded = 1;
	char* tempBuffer = 0;
	fseek(options->file, current, SEEK_CUR);
	int bufferSize = options->chunksCount * options->chunkType;
	tempBuffer = calloc(bufferSize, sizeof(char));
	if (tempBuffer == 0) return;
	int invalidBytesCount = bufferSize - options->chunkType * 2;
	if (invalidBytesCount < 0) invalidBytesCount = 0;
	int bytesToRead = (options->bytesCount > 0) ? options->bytesCount : INT_MAX;
	int i = 0;
	printf("\n");
	while (isReaded > 0) {
		int quote = max(min(bufferSize, bytesToRead), 0);
		int readedBytes = fread(tempBuffer, 1, quote, options->file);
		bytesToRead -= readedBytes;
		isReaded = (readedBytes > 0);
		if (isReaded == 0) break;
		printFormatString(i, current, tempBuffer, readedBytes, options);
		current += bufferSize;
		i++;
		for (int i = readedBytes-1; i >= invalidBytesCount; i--) {
			tempBuffer[i] = 0;
		}
	}

}

void displayFileAsHex(DisplayOptions* options)
{
	if (options->file == 0) {
		printf("\nError: Please provide file to read!\n");
		return;
	}
	if (options->format == 0) {
		displayClassic(options);
	}
	else {
		displayFormat(options);
	}
}
