// Препроцессор языка С


// подставляет содержимое
#include <stdio.h>


#include "file.h"
//#include "file.h"


#include "main.h"


// определение макроса
#define MAX_STRING_SIZE     (100 + 1)


#define min(a,b) a < b ? a : b




//#define max(a,b)        a < b ? b : a
#define max(a,b)        ((a) < (b) ? (b) : (a))
#define mul(a,b)        ((a) * (b))
//#define mul(a,b)        (a * b)

//#define for_each(buf, size, action) {int i;for (i = 0; i < size; ++i) {action (&buf[i]);}}

#define for_each(buf, size, action) \
    {\
    int i;\
    for (i = 0; i < size; ++i) {\
        action (&buf[i]);\
        }\
    }

#define DefineArray(type, name, size)   \
type name[size];

#define DefineString(name, str)\
char name[] = #str;

char str[] = "adsadf";
//DefineString (str1, str1) -> char str1[] = #str1    ->  char str1[] = "str1";


void init1 (int *value) {
    *value = 1;
}

void print_int (int *value) {
    printf ("%d\n", *value);
}



int MaxFun (int a, int b) {

    return a < b ? b : a;
}

 

int main () {

int a = 10;
int b = 1;
//int buf[3];// = {0, 0, 0};
DefineArray (int, buf, 3)
DefineString (str1, str1)

    //2 * 1 < 3 ? 1 : 3
    printf ("min %d\n", 2 * min(1,3));
    //return 0;
    //return 1;

    //printf ("%d\n", 2 * MAX_STRING_SIZE);
    //return 1;

    printf ("max(6, 10) = %d\n", max(6,10));
    printf ("2*max(6, 10) = %d\n", 2*max(6,10));  // 2 * 6 < 10 ? 10 : 6

    printf ("mul(2+2,2) = %d\n", mul(2+2,2));

    printf ("max(a, b) = %d\n", max (a, b));
    printf ("max(a++, b++) = %d\n", max (a++, b++));    // ((a++) < (b++) ? (b++) : (a++))
    printf ("max(a++, b++) = %d\n", max (a++, b++));

    printf ("%s\n", str1);

    for_each (buf, 3, init1);
    for_each (buf, 3, print_int);

    //printf ("max string size: %d\n", MAX_STRING_SIZE);
        

    return 0;
}

