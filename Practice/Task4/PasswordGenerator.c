#include "PasswordGenerator.h"
#include <stdlib.h>
#include <stdio.h>

static const char* alpha_a_z = "abcdefghijklmnopqrstuvwxyz";
static const char* alpha_A_Z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char* alpha_0_9 = "0123456789";
static const char* alpha_S = "!@#$%^&*()[]/.,";

#define ALPHABET_SETS_COUNT 4

#define MAX_PASSWORD_SIZE 9999

#define FLAG_DUPE "You can't dupliate this flag OR use it with [-n]: "
#define FLAG_DUPE_ALPHA "Wrong symbol OR duplicate in -C flag!"
#define FLAG_INVALID_VAL "Invalid value for flag: "
#define FLAG_INVALID_RANGE "Invalid range! "
#define FLAG_UNEXPECTED_RANGE "You can't use -m1 or -m2, with -n OR just duplicate -n flag!"
#define FLAG_UNEXPECTED_ALPHA "You can't use -a and -C together OR duplicate this flags!"

#define throw(msg, reserved, err_code) {printf("Error: %s (%s)\n", msg, reserved); return err_code;} 

/*
набор символов (указывается один или несколько символов из множества {a, A, D, S}),
a - маленькие латинские символы,
A - большие латинские символы, 
D - цифры,
S - спецсимволы.
*/

#include <math.h>

int randInt(int min, int max)
{
    int r = rand() ^ (rand() << 16);
    if (max < min || max == 0) return 0;
    return ((r % (max - min + 1)) + min);
}



NEW char* generatePassword(IN GenOptions* options)
{
    char** alpha_bucket[4] = { alpha_a_z, alpha_A_Z, alpha_0_9, alpha_S };
    char** alpha_sizes[4] = { 26, 26, 10, 15};
    srand(__security_cookie ^ (__int64)generatePassword);
    if (options->maxPasswordLength < 0 
        || options->minPasswordLength < 0
        || options->minPasswordLength > options->maxPasswordLength
        ) {
        throw("Error: please provide password length or VALID range","", 0);
    }
    int len = randInt(options->minPasswordLength, options->maxPasswordLength);
    char* result = calloc(len+1, sizeof(char));
    if (result == 0) return 0;
    char* alpha;
    int alpha_size;
    if (options->isAlphabetCustom == -1) {
        throw("Please provide alphabet (-a <symbols> or -C [aADS])", "", 0);
    }
    alpha = options->customAlphabet;
    alpha_size = options->customAlphabetSize;
    for (int i = 0; i < len; i++) {
        if (!options->isAlphabetCustom) {
            char* packed = &options->isLatinLower;
            int def_index = -2, alpha_index = -1, packed_c = 0;
            for (int i = 0; i < ALPHABET_SETS_COUNT; i++) {
                if (packed[i]) def_index = (packed_c++, i);
            }
            for (int i = 0; i < ALPHABET_SETS_COUNT; i++) {
                if (packed[i] && (rand() % packed_c) && i != def_index) {
                    alpha_index = i;
                    break;
                }
            }
            if (alpha_index == -1) alpha_index = def_index;
            alpha = alpha_bucket[alpha_index];
            alpha_size = alpha_sizes[alpha_index];
        }
        result[i] = alpha[randInt(0, alpha_size - 1)];
    }
    result[len] = '\0';
    return result;
}

#include <string.h>

/*
-m1	        минимальная длина пароля
-m2	        максимальная длина пароля
-n          длина пароля
-c          количество паролей
-a          алфавит символов
-C [aADS]	набор символов
*/

NEW char* CopyString(char* source, OUT int* retSize) {
    
    int len = strlen(source);
    char* copy = calloc(len + 1, sizeof(char));
    if (copy == 0) {
        printf("\n\nFATAL ERROR: Can't alloc Memory\n\n");
        exit(-10);
        return 0;
    }
    copy[len] = '\0';
    for (int i = 0; i < len; i++) {
        copy[i] = source[i];
    }
    if (retSize != 0) *retSize = len;
    return copy;
}

static int handlePasswordLength(
    int args_c, IN char** argv, OUT GenOptions* options, int i
) {
    if (strcmp(argv[i], "-m1") == 0) {
        if (options->minPasswordLength != -1) {
            throw(FLAG_DUPE, "-m1", -2);
        }
        int tmp = atoi(argv[i + 1]);
        if (tmp < 1 || tmp > MAX_PASSWORD_SIZE) {
            throw(FLAG_INVALID_VAL, "-m1", -3);
        }
        options->minPasswordLength = tmp;
    }
    if (strcmp(argv[i], "-m2") == 0) {
        if (options->maxPasswordLength != -1) {
            throw(FLAG_DUPE, "-m2", -2);
        }
        int tmp = atoi(argv[i + 1]);
        if (tmp < 1 || tmp > MAX_PASSWORD_SIZE) {
            throw(FLAG_INVALID_VAL, "-m2", -3);
        }
        options->maxPasswordLength = tmp;
    }
    if (strcmp(argv[i], "-n") == 0) {
        if (options->maxPasswordLength != -1 ||
            options->minPasswordLength != -1) {
            throw(FLAG_UNEXPECTED_RANGE, "", -1);
        }
        int tmp = atoi(argv[i + 1]);
        if (tmp < 1 || tmp > MAX_PASSWORD_SIZE) {
            throw(FLAG_INVALID_VAL, "-n", -3);
        }
        options->maxPasswordLength = tmp;
        options->minPasswordLength = tmp;
    }
    return 0;
}

static int handlePasswordAlphabet(
    int args_c, IN char** argv, OUT GenOptions* options, int i, char* isCustom
) {
    if (strcmp(argv[i], "-a") == 0) {
        if (options->isAlphabetCustom != -1) {
            throw(FLAG_UNEXPECTED_ALPHA, "", -2);
        }
        options->isAlphabetCustom = 1;
        options->customAlphabet = CopyString(argv[i + 1], &options->customAlphabetSize);
        if (isCustom != 0) *isCustom = 1;
    }
    if (strcmp(argv[i], "-C") == 0) { //aADs = 
        if (options->isAlphabetCustom != -1) {
            throw(FLAG_UNEXPECTED_ALPHA, "", -2);
        }
        options->isAlphabetCustom = 0;
        int j = 0;
        int* packed = (int*)(&options->isLatinLower);
        while(argv[i + 1][j] != '\0') {
            if (argv[i + 1][j] == 'a') options->isLatinLower++;
            if (argv[i + 1][j] == 'A') options->isLatinUpper++;
            if (argv[i + 1][j] == 'D') options->isDigitsUsed++;
            if (argv[i + 1][j] == 'S') options->isSpecialUsed++;
            int check = options->isLatinLower ^ options->isLatinUpper ^
                options->isDigitsUsed ^ options->isSpecialUsed;
            if (check == 2 || *packed == 0) {
                throw(FLAG_DUPE_ALPHA, "", -3);
            };
            j++;
        }
    }
    return 0;
}

int readFlags(int args_c, IN char** argv, OUT GenOptions* options)
{
    options->minPasswordLength = -1;
    options->maxPasswordLength = -1;
    options->customAlphabet = 0;
    options->customAlphabetSize = 0;
    options->isAlphabetCustom = -1;
    *((int*)(&options->isLatinLower)) = 0;
    for (int i = 1; i < args_c; i++) {
        char* ref = argv[i];
        if (ref[0] == '-' && i == args_c - 1){
            return 0;
        }
        if (ref[0] != '-') {
            continue;
        }
        if (argv[i + 1][0] == '-' && i + 2 < args_c) {
            if (argv[i + 2][0] < '0' && argv[i + 2][0] > '9') {
                continue;
            }
        }
        if (strcmp(argv[i], "__IGNORED__") == 0) {
            continue;
        }
        int status = 1;
        char isCustomAlphabet = 0;
        if (handlePasswordLength(args_c, argv, options, i) != 0) status = 0;
        if (handlePasswordAlphabet(args_c, argv, options, i, &isCustomAlphabet) != 0) status = 0;
        if (isCustomAlphabet) { i++; continue; };
        if (strcmp(argv[i], "-c") == 0) {
            int tmp = atoi(argv[i + 1]);
            if (tmp < 1) throw(FLAG_INVALID_VAL, "-c", 5);
            options->passwordsCount = tmp;
        }
        if (status == 0) return 1;
    }
    return 0;
}
