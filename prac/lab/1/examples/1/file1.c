#include <stdio.h>

//static char *glStr = "Fun execute\n";

extern char *glStr;

void fun (void) {

    printf ("%s", glStr);
    return;
}
