
#include <stdio.h>

// целочисленные типы
char charVar;               // 0000 0000 = 0 - 1111 1111 = -1
unsigned char uCharVar;     // 0000 0000 = 0 - 1111 1111 = 255
                            // 1111 1111 + 0000 0001 = 1 0000 0000
short shorVar;
unsigned short uShortVar;
int intVar;
unsigned int uIntVar;
long longVar;
unsigned long uLongVar;
long long lonLongVar;

// вещественные типы
float floatVar;
double doubleVar;
long double longDoubleVar;

// указатели
char *pChar;    // тип указатель на char
int *pInt, a;

// массивы
char charBuf[10];   // тип массив из 10 элементов типа char
char c1 = -0x10;
char c2 = '0';   // 0x30 = 48
char charBuf2[10] = {'a', 'b', 'c'};    // 'a' -> число, равное ASCII-коду символа a
int intBuf[] = {1, 2, 3};
char charBuf3[4] = {'b', 'u', 'f', '3'};
char charBuf4[] = "buf4";


char *str = "string";
// в памяти располагается строка string\0, 
// в str будем помещен указатель на нулевой элемент (т.е. его адрес)

char *str1 = '1234';  // в str1 поместиться значение 0x31323334



int main (int argc, char *argv[], char *envp[]) {

    printf ("charBuf3: %d\n", sizeof (charBuf3));
    printf ("charBuf4: %d\n", sizeof (charBuf4));

    printf ("size(char) = %d\nsize(short) = %d\nsize(int) = %d\nsize(long) = %d\nsize(size_t) = %d\nsize(long long) = %d\n",
            sizeof (char), sizeof (short), sizeof (int), sizeof (long), sizeof(size_t), sizeof(long long));
    printf ("size(float) = %d\nsize(double) = %d\nsize(long double) = %d\n",
            sizeof (float), sizeof (double), sizeof (long double));

    printf ("%d\n", charBuf[100]);
    charBuf[100] = 111;
    printf ("%d\n", charBuf[100]);

    return 0;
}


