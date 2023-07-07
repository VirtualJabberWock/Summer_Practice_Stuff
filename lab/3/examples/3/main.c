#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define IN
#define OUT

int glBuf1[] = {1, 2};
int glBuf2[100000000];


void fun (void) {

    static int count = 0;
    //int a;
    printf ("%d\n", ++count);
    fun();
}



char GetElementOfMultiBufByIndexPair (char *multiBuf, int i, int j, int rowSize) {

    return multiBuf[i * rowSize + j];
}



char GetElementOfMultiBufByIndexPair2 (char multiBuf[][20], int i, int j) {

    return multiBuf[i][j];  // -> p[i * 20 + j]
}

char c;
char *p = &c;




int main (int argc, char *argv[], char *envp[]) {

int buf1[] = {1, 2, 3, 4, 5};
int buf2[200000];
char buf3[] = "1234"; // -> {'1', '2', '3', '4', 0}
char buf4[4] = "1234"; // -> {'1', '2', '3', '4'}
char *pChar;
char *pCharDynBuf;
int *pIntDynBuf;
//int *printf ("%c\n", GetElementOfMultiBufByIndexPair2 (multiBuf, 5, 9));
char multiBuf[10][20];
char multiBuf2[10][40];
char *****p;
//char variableSizeArray[argc];



    /*
    p[10] -> *(p+10);

    a[b] -> *(a+b) -> *(b+a) -> b[a]
    buf1[1] -> 1[buf1]

    buf1 == &buf1;
    p = buf1;
    p == &p;
    buf1[10] -> *(buf1+10)
    */
    p;
    p[1];
    1[p];
	1[buf1];

    //buf1 = p;
    p = buf1;
    p;
    &p;

    //p[0][1] -> (*(p+0))[1] -> *((*(p+0))+1)


	//buf3 = 0;
	//*buf3 = '9';
    //buf3++; // не lvalue, а rvalue
    //*(++buf3) = '9';     // так можно
    //*(pChar++) = '9';      // так нельзя

    pChar = buf3;
    printf ("%c\n", *pChar);
    pChar = &buf3;
    printf ("%c\n", *pChar);
    pChar = &buf3[0];
    printf ("%c\n", *pChar);

    printf ("%c\n", buf3[1]);   // *(buf3 + 1)
    printf ("%c\n", 1[buf3]);   // *(1 + buf3)
    printf ("%c\n", pChar[1]);  // *(pChar + 1)
    printf ("%c\n", 1[pChar]);  // *(1 + pChar)

    //fun();

    printf ("Sizeof(buf3) = %d\n", sizeof(buf3));
    printf ("Sizeof(pChar) = %d\n", sizeof(pChar));

    pCharDynBuf = (char*) malloc (10 * sizeof(int));
    //pCharDynBuf = (char*) calloc (10, sizeof(int));
    pIntDynBuf = (int*) pCharDynBuf;

    pCharDynBuf[0] = '0';
    pCharDynBuf[1] = '1';
    pCharDynBuf[2] = '2';
    pCharDynBuf[3] = '3';
    printf ("%08X\n", pIntDynBuf[0]); // 0x33323130

    pIntDynBuf[3] = 1;
    // ->*(pIntDynBuf+3) = 1;   -> int *tmp = addr(pIntDynBuf) + 3 * sizeof(int); *tmp = 1

    {
    char c1;
    char c2;
    char *p1 = &c1;
    char *p2 = &c2;
    char *p = p1 - p2;

    /*
    type *p;
    p = p +- n;    // (type*) addr (p) +- n * sizeof(type)
    */

    }

    printf ("%d\n", pIntDynBuf[5]);

    multiBuf[5][9] = 'A';
    printf ("%c\n", GetElementOfMultiBufByIndexPair (multiBuf, 5, 9, 20));
    printf ("%c\n", GetElementOfMultiBufByIndexPair2 (multiBuf, 5, 9));

    multiBuf2[5][9] = 'B';
    printf ("%c\n", GetElementOfMultiBufByIndexPair (multiBuf2, 5, 9, 40));
    printf ("%c\n", GetElementOfMultiBufByIndexPair2 (multiBuf2, 5, 9));

    getchar();


    return 0;
}
