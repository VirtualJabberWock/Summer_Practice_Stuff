#include <stdio.h>
#include "prog2.h"

// определение глобальной переменной
//(её имя будет помещено в таблицу внутренних символов объектного файла)
int glVarProg2;

static void fun1 (void) {
    
    printf ("fun1 in prog2.c\n");
}

void fun (void) {

    printf ("Hello, World!\n");
    
    fun1();
    
    return;
}


