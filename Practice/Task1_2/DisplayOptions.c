#include "DisplayOptions.h"
#include <stdlib.h>
#include "getopt.h"
#include <string.h>
#include "Format.h"

static OnFlagReadError handleHelpFlagAndExit(char* reserved, DisplayOptions* ptr, char flag) {
	printf("\n");
	printf("Help(Flags):\n");
	printf("\n\t-h\tshow this help page          \tOPTIONAL*\n");
	printf("\n\t-i\tpath to file to open for read\tREQUIRED!\n");
	printf("\n\t-l\tnumber of bytes to display   \tOPTIONAL*\n");
	printf("\n\t-o\toffset from start of file    \tOPTIONAL*\n");
	printf("\n\t-g\tgroup bytes into chunks:     \tOPTIONAL*\n\n");
	printf("\t\t'byte'  - group by 1 byte\n");
	printf("\t\t'word'  - group by 2 byte\n");
	printf("\t\t'dword' - group by 4 byte\n");
	printf("\t\t'qword' - group by 8 byte\n");
	printf("\n\t-n\tcount of chunks on one line  \tOPTIONAL*\n");
	printf("\n\t-f\tformat of displaying bytes:  \tOPTIONAL*\n\n");
	printf("\t\t%ci   - current line index\n", '%');
	printf("\t\t%cn   - current offset\n", '%');
	printf("\t\t%cix  - print i-th group in hex format \n", '%');
	printf("\t\t%cic  - print i-th group in ASCII format\n", '%');
	printf("\n");
	exit(0);
	return NO_ERR;
}

static OnFlagReadError handleNumberFromFlag(char* flagData, int* outNumber, char flag) {
	if (flagData == 0) return NULL_DATA;
	int i = 0;
	int number = atoi(flagData);
	if (number < 0) return ERR_NEGATIVE_VALUE;
	while (flagData[i] != '\0') {
		if (flagData[i] < '0' || flagData[i] > '9')
			return ERR_INVALID_NUMBER;
		i++;
	}
	if (flag == 'n' && number > 256) return ERR_N_FLAG_LIMIT;
	*outNumber = number;
	return NO_ERR;
}
  
static OnFlagReadError handleFileFromFlag(char* filename, DisplayOptions* options, char flag) {
	if (filename == 0) return NULL_DATA;
	int status = fopen_s(&options->file, filename, "rb");
	if (status != 0) return ERR_INVALID_FILE;
	if (options->file == 0) return ERR_INVALID_FILE;
	return NO_ERR;
}

static OnFlagReadError handleGroupFromFlag(char* group, DisplayOptions* options, char flag) {
	if (group == 0) return NULL_DATA;
	if (group[0] == 0) return ERR_INVALID_GROUP;
	if (strcmp(group, "byte") == 0)  return (options->chunkType = BYTE,  NO_ERR);
	if (strcmp(group, "word") == 0)  return (options->chunkType = WORD,  NO_ERR);
	if (strcmp(group, "dword") == 0) return (options->chunkType = DWORD, NO_ERR);
	if (strcmp(group, "qword") == 0) return (options->chunkType = QWORD, NO_ERR);
	return ERR_INVALID_GROUP;
}

static OnFlagReadError handleFormatFromFlag(char* format, DisplayOptions* options, char flag) {
	if (format == 0) return NULL_DATA;
	if(isFormatValid(format, &options->formatSize, options->chunkType) != NO_ERR)
		return ERR_INVALID_FORMAT;
	options->format = calloc(options->formatSize + 1, sizeof(char));
	if (options->format == 0) return NULL_DATA;
	for (int i = 0; i < options->formatSize; i++) {
		options->format[i] = format[i];
	}
	options->format[options->formatSize] = 0;
	return NO_ERR;
}

static int handleFlagError(OnFlagReadError err_code, char lastFlag, char* value) {
	printf("\n");
	if (err_code == NULL_DATA) {
		printf("Error: NULL DATA.\n");
	}
	if (err_code == ERR_NEGATIVE_VALUE) {
		printf("Error: (-%c) value should be positive.\n", lastFlag);
	}
	if (err_code == ERR_INVALID_NUMBER) {
		printf("Error: (-%c) value should be valid number.\n", lastFlag);
	}
	if (err_code == ERR_INVALID_FILE) {
		printf("Error: file (%s) don't exsist, or can't be open.\n", value);
	}
	if (err_code == ERR_INVALID_GROUP) {
		printf("Error: (%s) is not valid group. See help for possible values.\n", value);
	}
	if (err_code == ERR_INVALID_FORMAT) {
		printf("Error: wrong use of -f flag.\n");
	}
	if (err_code == ERR_N_FLAG_LIMIT) {
		printf("Error: -n should be less than 256!\n");
	}
	printf("\nAlso use -h flag to show help!\n");
	return 0;
}

DisplayOptions* getOptionsFromFlags(int argc, char** argv)
{
	DisplayOptions* options = calloc(1, sizeof(DisplayOptions));
	if (options == 0) {
		printf("FATAL ERROR: Can't alloc memory!\n");
		return 0;
	}
	options->chunksCount = -1;
	options->chunkType = BYTE;
	options->file = 0;
	options->offset = 0;
	options->format = 0;
	int c = 0, s = 0;
	while ((c = getopt(argc, argv, "hi:o:l:g:n:f:")) != -1) {
		switch (c) {
		case 'h':
			handleHelpFlagAndExit(0, options,c);
			break;

		case 'i':
			s = handleFileFromFlag(optarg, options,c);
			break;

		case 'o':
			s = handleNumberFromFlag(optarg, &options->offset,c);
			break;

		case 'l':
			s = handleNumberFromFlag(optarg, &options->bytesCount,c);
			break;

		case 'g':
			s = handleGroupFromFlag(optarg, options,c);
			break;

		case 'n':
			s = handleNumberFromFlag(optarg, &options->chunksCount,c);
			break;

		case 'f':
			s = handleFormatFromFlag(optarg, options,c);
			break;

		default:
			printf("Invalid option, use -h to show help\n");
			disposeDisplayOptions(&options);
			return 0;
		}
		if (s != NO_ERR) {
			disposeDisplayOptions(&options);
			return handleFlagError(s, c, optarg);
		}
	}
	return options;
}

void disposeDisplayOptions(DisplayOptions** ptrOptions)
{
	if (ptrOptions == 0) return;
	if (*ptrOptions == 0) return;
	FILE* f = (*ptrOptions)->file;
	if (f != 0) fclose(f);
	free(*ptrOptions);
	*ptrOptions = 0;
}
