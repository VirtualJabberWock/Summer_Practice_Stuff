#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int getDigit(char c) {
    if (c >= '0' || c <= '9')
        return c - '0';
    return c;
}

int strtoi(char* str, char** p, int* ret) {

    int base = 16;
    int len = strlen(str);
    if (len == 0) {
        *p = (char*)str;//передаём основание СС и выходим
        return 1;
    }
    int result, limit, remain;
    unsigned int MAX_SIGNED_VALUE;

    if (1) {
        MAX_SIGNED_VALUE = abs(INT_MIN);
    }
    else {
        MAX_SIGNED_VALUE = INT_MAX;
    }

    limit = MAX_SIGNED_VALUE / base;
    remain = MAX_SIGNED_VALUE % base;
    result = 0;

    for (int i = 0; i < len; i++) {
        int digit = getDigit(str[i]);
        if (digit == -1) {
            *p = (char*)(str + 1);
            return 1;
        }

        if (result > limit || (result == limit && digit > remain)) {
            return 2;
        }

        result = result * base;

        if (MAX_SIGNED_VALUE - result < digit) {
            return 2;
        }

        result = result + digit;
    }
    if (1) {
        *ret = result * -1;
    }
    else {
        *ret = result;
    }
    return 0;
}

int main() {

    int t = 0;

    strtoi((char*)"80000000", 0, &t);
    printf("%d", t);
}