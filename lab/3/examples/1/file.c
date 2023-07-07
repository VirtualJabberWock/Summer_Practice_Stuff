#include <stdio.h>

#include "file.h"

void UseTestStruct (struct TestStruct *ts) {

    printf ("a = %d\nb = %d\n", ts->a, ts->b);

    return;
}
