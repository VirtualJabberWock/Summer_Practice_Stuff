// Шаблон задания 3.2

#include <stdlib.h>

typedef unsigned int *BigNum;

#define IN
#define OUT


// выделяет память для хранения большо числа и инициализирует его из строки
// шестнадцатиричных символов.
BigNum GetBigNumByStr (IN const char *str, OUT size_t *bigNumSize) {

BigNum bigNum;
//BigNum bigNum = malloc (...);

    return bigNum;
}



void PrintBigNum (IN BigNum bigNum, size_t bigNumSize) {


}


void AddBigNum (IN BigNum bigNum1, IN BigNum bigNum2, OUT BigNum res, size_t bigNum1Size, size_t bigNum2Size) {


}

SubBigNum

MulBigNum

DivBigNum


int main (int argc, char *argv[]) {

size_t bigNum1Size;
BigNum bigNum1 = GetBigNumByStr (argv[1], &bigNum1Size);

    //AddBigNum (..);
    //PrintBigNum (res);

    return 0;
}
