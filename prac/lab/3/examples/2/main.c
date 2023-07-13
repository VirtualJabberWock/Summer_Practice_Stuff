#include <stdio.h>

#pragma pack(1)
struct NameStruct {
    int intField;   // 0x34333231
    char charField; // 0x35
    short shortField; //0x36
};


int main () {

char *str = "123456";
char buf[10][20][30][40];

char *p = (char*)&buf;

struct NameStruct *ns = (struct NameStruct*) str;

short *shortBuf = (short*) str;

    buf[1][2][3][4] = 'p';
    printf ("%c\n", p[1*20*30*40 + 2*30*40 + 3*40 + 4]);

    printf ("intField = %x\n", ns->intField);       // *((int*)(ns + 0))
    printf ("charField = %c\n", ns->charField);     // *((char*)(ns + 4))
    printf ("shortField = %x\n", ns->shortField);   // *((short*)(ns + 5))
    
    printf ("shortBuf[2] = %x\n", shortBuf[2]);

    return 0;
}
