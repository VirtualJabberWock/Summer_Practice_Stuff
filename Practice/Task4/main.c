#include "PasswordGenerator.h"
#include <stdio.h>
#include <stdlib.h>

//C:\Users\aquahaze\Documents\GitHub\Summer_Practice_Stuff\Practice\x64\Debug>Task4.exe

int main(int argc, char** argv) {
	GenOptions options;
	readFlags(argc, argv, &options);
	char* newPassword = generatePassword(&options);
	printf("\npass = %s\n", newPassword);
	free(newPassword);
}