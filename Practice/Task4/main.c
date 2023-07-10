#include "PasswordGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include "PasswordTests.h"
#include "Kludges.h"

//r>Task4.exe

#define IS_MAIN_HOOKED 0 //For debug crashes!

#if IS_MAIN_HOOKED
#define HOOK(func, ...) __hooked_##func(__VA_ARGS__)
#define LINK_HOOK(func, ...) func(__VA_ARGS__)
#else
#define HOOK(func, ...) func(__VA_ARGS__)
#define LINK_HOOK(func, ...) __hooker_##func(__VA_ARGS__)
#endif

#define _IS_LOCALE_PROTECT_ACTIVE 1

int HOOK(main, int argc, char** argv) {

	if (argc > 1) {

		initKludges();

		PassGenOptions* options = CreateEmptyGenOptions();
		int isFlagsCorrupted = readFlags(argc, argv, options);
		if (isFlagsCorrupted) return printf("Also use flag -help to show help.\n");
		srand((int)options);
		if (options->passwordsCount > 1) {
			for (int i = 0; i < options->passwordsCount; i++) {
				srand(rand() ^ i * 13);
				char* newPassword = generatePassword(options);
				if (newPassword == 0) return printf("Also use flag -help to show help.\n");
				printf("\n[%d] pass = %s", i, newPassword);
				free(newPassword);
			}
		}
		else {
			char* newPassword = generatePassword(options);
			if (newPassword == 0) return printf("Also use flag -help to show help.\n");
			printf("\npass = %s\n", newPassword);
			free(newPassword);
		}
		free(options);
	}
	else {

		showGeneratePasswordTest();

	}
}

int LINK_HOOK(main, int _argc, char** _argv) {
	if (_argc > 1) {
		return HOOK(main, _argc, _argv);
	}
	int argc = 2;
	char** test_argv = (char*)calloc(11, sizeof(char*));
	test_argv[0] = CopyString("%PATH%", 0);
	test_argv[1] = CopyString("-help", 0);
	HOOK(main, argc, test_argv);
	for (int i = 0; i < argc; i++) {
		free(test_argv[i]);
	}
	free(test_argv);
}

