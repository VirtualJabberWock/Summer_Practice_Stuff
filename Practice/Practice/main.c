#include <stdio.h>
#include "ParseIntTests.h"
#include "ParseInt.h"
#include <Windows.h>

#define IS_USER_TEST 1

int main(int argc, char** argv) {

#if IS_USER_TEST

	char buffer[512] = { 0 };

	while (1) {

		printf("\n\"stop\" - to stop!\n");
		printf("Input number(max 512 chars): ");
		int s = scanf_s("%s", buffer, 512);
		if (s == 0) {
			printf("\nBad string format!");
			return -1;
		}
		if (((int*)(buffer))[0] == 'pots') break;
		char* badChar;
		int number = 0;
		int status = strtoi(buffer, &badChar, &number);
		if (status == 0) {
			printf("Parsed number(to base10): %d\n\n", number);
			char* itoaBuffer = calloc(512, sizeof(char));
			if (itoaBuffer == 0) return -1;
			int base_get = getDigitOrdinal(buffer[0]);
			int neededbufferSize = 0;
			int base = 10;
			if (base_get > 0 || base_get < 63 && buffer[1] == 'x') base = base_get;
			neededbufferSize = myitoa(itoaBuffer, 512, number, base);
			printf("myitoa(...) result: %s (returned bufferSize: %d)\n\n", itoaBuffer, neededbufferSize);
			free(itoaBuffer);
		}
		if (status == STRTOI_ERR_NULL) {
			printf("Error: NULL\n\n");
		}

		if (status == STRTOI_ERR_BAD_CHAR) {
			printf("Error: Bad char [%c] at %d pos in str: %s\n\n", *badChar, (int)(badChar - buffer), buffer);
		}

		if (status == STRTOI_ERR_OVERFLOW) {
			printf("Error: the number is out of range (-2^31 to 2^31-1): %s\n\n", buffer);
		}

		

	}

#else

	COORD size;
	size.Y = 2000;
	SetConsoleScreenBufferSize(GetConsoleOutputCP(), size);

	showStrToIntTests();
	showMyIntToStrTests();

#endif

	return 0;
}
