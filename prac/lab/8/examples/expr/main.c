/*
Пример разбора арифметических выражений с помощью регулярных выражений.

Маткин Илья Александрович  15.07.2015
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcre.h"
#include "pcreposix.h"

//----------------------------------------

typedef struct _StringMapEntry {

    struct _StringMapEntry *next;
    char *key;
    char *value;
    
} StringMapEntry;

typedef struct _StringMap {
    
    StringMapEntry *first;

} StringMap;

typedef StringMap VariablesContainer;


//----------------------------------------


VariablesContainer glVars;

//----------------------------------------


void InitStringMap (StringMap *map) {

    map->first = NULL;

    return;
}



StringMapEntry *CreateAndInitStringMapEntry (const char *key, const char *value) {
    
StringMapEntry *entry;

    entry = (StringMapEntry*) malloc (sizeof(StringMapEntry));
    if (!entry) {
        return NULL;
        }

    entry->key = (char*) malloc (strlen (key) + 1);
    if (!entry->key) {
        free (entry);
        return NULL;
        }
    entry->value = (char*) malloc (strlen (value) + 1);
    if (!entry->value) {
        free (entry->key);
        free (entry);
        return NULL;
        }
    strcpy (entry->key, key);
    strcpy (entry->value, value);

    return entry;
}



void InsertStringMap (StringMap *map, const char *key, const char *value) {

StringMapEntry *newEntry;

    newEntry = CreateAndInitStringMapEntry (key, value);
    if (!newEntry) {
        return;
        }

    newEntry->next = map->first;
    map->first = newEntry;

    return;
}



const char * GetStringByKey (StringMap *map, const char *key) {

StringMapEntry *entry = map->first;

    while (entry) {
        if (!strcmp (entry->key, key)) {
            return entry->value;
            }
        entry = entry->next;
        }

    return NULL;
}


#define MAX_MATCH   10
void ParseLine (const char *line) {

regex_t re;
regmatch_t match[MAX_MATCH];

    memset (match, 0, sizeof(match));


    if (!regcomp (&re, "^\\s*([a-zA-Z][a-zA-Z0-9_]*)\\s*=\\s*([0-9]+)\\s*;\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {
            char *varName;
            char *initNumber;
            varName = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
            initNumber = (char*) malloc (match[2].rm_eo - match[2].rm_so + 1);

            memcpy (varName, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            varName[match[1].rm_eo - match[1].rm_so] = 0;

            memcpy (initNumber, line + match[2].rm_so, match[2].rm_eo - match[2].rm_so);
            initNumber[match[2].rm_eo - match[2].rm_so] = 0;

            InsertStringMap (&glVars, varName, initNumber);

            printf ("%s = %d\n", varName, atoi (initNumber));

            free (varName);
            free (initNumber);
            }
        regfree (&re);
        }

    if (!regcomp (&re, "^\\s*([a-zA-Z][a-zA-Z0-9]+)\\s*\\+\\s*([0-9]+)\\s*;\\s*$", 0)) {
        if (!regexec (&re, line, MAX_MATCH, match, 0)) {
            char *varName;
            char *initNumber;
            const char *varValue;
            varName = (char*) malloc (match[1].rm_eo - match[1].rm_so + 1);
            initNumber = (char*) malloc (match[2].rm_eo - match[2].rm_so + 1);

            memcpy (varName, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            varName[match[1].rm_eo - match[1].rm_so] = 0;

            memcpy (initNumber, line + match[2].rm_so, match[2].rm_eo - match[2].rm_so);
            initNumber[match[2].rm_eo - match[2].rm_so] = 0;

            varValue = GetStringByKey (&glVars, varName);
            if (varValue) {
                printf ("%s + %d = ", varName, atoi (initNumber));
                printf ("%d\n", atoi (varValue) + atoi (initNumber));
            }

            free (varName);
            free (initNumber);
            }
        regfree (&re);
        }

    return;
}



void ParseFile (const char *fileName) {

char line[1024];
FILE *fd;
    
    fd = fopen (fileName, "r");
    while (fgets (line, 1024, fd)) {
        ParseLine (line);
        }
 
    fclose (fd);

    return;
}


int main (int argc, char *argv[], char *envp[]) {


    InitStringMap (&glVars);
    ParseLine ("   var1   =   11  ;   ");
    ParseLine ("  var1   +   10  ;   ");
    //ParseLine ("  1   +   10  ;   ");
    // ParseLine ("  var1 + var2  ;   ");



    if (argc < 2) {
        printf ("usage: %s filename\n", argv[0]);
        return 1;
        }

    ParseFile (argv[1]);

    return 0;
}
