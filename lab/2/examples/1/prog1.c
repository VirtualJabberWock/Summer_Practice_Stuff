#include "prog2.h"

//int glVarProg2;

// статическая функция видна только в пределах модуля
// (не помещается в таблицу внутренних символов объектного файла)
void fun1 (void) {
    
    printf ("fun1 in prog1.c\n");
}



int main (int argc, char *argv[], char *envp[]) {
    
    fun ();
    
    fun1();
    
    // использование глобальной переменной, определённой в другом модуле
    // (будет помещена в таблицу внешних символов объектного файла)
    printf ("glVarProg2 = %d\n", glVarProg2);
    
    return 0;
}

