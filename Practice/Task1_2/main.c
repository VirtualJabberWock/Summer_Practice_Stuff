#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "DisplayOptions.h"
#include "HexDisplay.h"



int main(int argc, char* argv[], char* envp[]) {

	DisplayOptions* options = getOptionsFromFlags(argc, argv);

	if (options == 0) return -1;

	displayFileAsHex(options);

	disposeDisplayOptions(&options);

	return 0;

}