#include <stdio.h>

#include "add_int.h"
#include "add_double.h"


int main () {

    printf ("%d\n", AddInt (10, 20));
    printf ("%f\n", AddDouble (1.0, 2.0));

    return 0;
}
