/*
Пример использования библиотеки PCRE.

Маткин Илья Александрович  15.07.2015
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcre.h"
#include "pcreposix.h"



int main () {

regex_t re;
char *data = "asdfadsfa108.102.103.104adsfad467878979kj;lkjkj";
regmatch_t match[10];
int i;

    memset (match, 0, sizeof(match));


    //if (!regcomp (&re, "^[^[:digit:]]*([[:digit:]]{1,3})\\.([0-9]+)\\.([0-9]+)\\.([0-9]+)", 0)) {
    if (!regcomp (&re, "^[^[:digit:]]*(([[:digit:]]{1,3})\\.([0-9]+)\\.([0-9]+)\\.([0-9]+))[^[:digit:]]*([0-9]?+)(.*)$", 0)) {
    //if (!regcomp (&re, "^[^[:digit:]]*([[:digit:]]{1,3}).([0-9]+)\\.([0-9]+)\\.([0-9]+)", 0)) {
        if (!regexec (&re, data, 10, match, 0)) {
            for (i = 0; i < 10; ++i) {
                char tmp[100];
                unsigned int startIndex = match[i].rm_so;
                unsigned int endIndex = match[i].rm_eo;
                unsigned int matchSize = endIndex - startIndex;
                if (startIndex < 0) {
                    break;
                    }
                memcpy (tmp, data + startIndex, matchSize);
                tmp[matchSize] = 0;
                printf ("%s ", tmp);
                }
            printf ("\n");
            }
        else {
            printf ("no match\n");
            }
        
        regfree (&re);
        }

    return 0;
}
