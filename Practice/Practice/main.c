#include <stdio.h>
#include "ParseIntTests.h"
#include "ParseInt.h"



int main(int argc, char** argv) {

	/*printf("Input number(max 1000 chars): ");
	char buffer[1000] = { 0 };
	scanf_s("%s", buffer, 1000);
	char* badChar;
	int number = 0;
	int status = strtoi(buffer, &badChar, &number);
	if (status == 0) {
		printf("Parsed number(to base10): %d\n\n", number);
	}
	if (status == STRTOI_ERR_NULL) {
		printf("Error: NULL\n\n");
	}

	if (status == STRTOI_ERR_BAD_CHAR) {
		printf("Error: Bad char [%c] at %d pos in str: %s\n\n", *badChar, (int)(badChar - buffer), buffer);
	}

	if (status == STRTOI_ERR_OVERFLOW) {
		printf("Error: the number is out of range (-2^31 to 2^31-1): %s\n\n", buffer);
	}*/

	showStrToIntTests();
	showMyIntToStrTests();
}