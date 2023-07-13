#include <stdio.h>

#include "polynom_int.h"



int main () {

    Polynom * pol1 = CreateIntPolynom ();

    // испльзование pol1

    DeleteIntPolynom (pol1);


    /*
    Polynom pol2;
    if (!InitPolynom (&pol2, 20)) {
        // error
    }
    */

    return;
}
