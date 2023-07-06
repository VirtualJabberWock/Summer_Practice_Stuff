#include "PasswordGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include "PasswordTests.h"

//C:\Users\aquahaze\Documents\GitHub\Summer_Practice_Stuff\Practice\x64\Debug>Task4.exe

#define IS_MAIN_HOOKED 0 //For debug crashes!

#if IS_MAIN_HOOKED
#define HOOK(func, ...) __hooked_##func(__VA_ARGS__)
#define LINK_HOOK(func, ...) func(__VA_ARGS__)
#else
#define HOOK(func, ...) func(__VA_ARGS__)
#define LINK_HOOK(func, ...) __hooker_##func(__VA_ARGS__)
#endif

int HOOK(main, int argc, char** argv) {

	if (argc > 1) {

		GenOptions options;
		readFlags(argc, argv, &options);
		char* newPassword = generatePassword(&options);
		if (newPassword == 0) return 0;
		printf("\npass = %s\n", newPassword);
		free(newPassword);

	}
	else {

		showGeneratePasswordTest();

	}
}

int LINK_HOOK(main, int __argc, char** __argv) {
	int argc = 5;
	char** test_argv = (char*)calloc(11, sizeof(char*));
	test_argv[0] = CopyString("%PATH%", 0);
	test_argv[1] = CopyString("-n", 0);
	test_argv[2] = CopyString("10", 0);
	test_argv[3] = CopyString("-a", 0);
	test_argv[4] = CopyString("-m2", 0);
	HOOK(main, argc, test_argv);
	for (int i = 0; i < argc; i++) {
		free(test_argv[i]);
	}
	free(test_argv);
}

