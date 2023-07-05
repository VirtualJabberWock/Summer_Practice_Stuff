#include "PasswordGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include "PasswordTests.h"

//C:\Users\aquahaze\Documents\GitHub\Summer_Practice_Stuff\Practice\x64\Debug>Task4.exe


int main(int argc, char** argv) {

	if (argc > 1) {

		GenOptions options;
		readFlags(argc, argv, &options);
		char* newPassword = generatePassword(&options);
		printf("\npass = %s\n", newPassword);
		free(newPassword);

	}
	else {

		showGeneratePasswordTest();

	}
}