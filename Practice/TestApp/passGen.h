#pragma once
char** getPasswords(int argc, char** argv, int* errCode);
char* concat(char* f, char* s);
char* getAlphabet(char* potenAlpha);
char* generatePassword(int minLen, int maxLen, int len, char* alphabet);

#define  ERR_WITH_LENGTH 1
#define ERR_WITH_ALPHA 2
#define ERR_WITH_PASSCOUNT 3
