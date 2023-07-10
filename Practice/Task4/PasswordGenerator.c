#include "PasswordGenerator.h"
#include <stdlib.h>
#include <stdio.h>

#include "StringUtil.h"

static const char* alpha_a_z = "abcdefghijklmnopqrstuvwxyz";
static const char* alpha_A_Z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char* alpha_0_9 = "0123456789";
static const char* alpha_S = "!#$%&()*+,-./:;<=>?@[\\]^_`{|}~";

#define ALPHABET_SETS_COUNT 4

#define MAX_PASSWORD_SIZE 9999

#define FLAG_DUPE "You can't dupliate this flag OR use it with [-n]: "
#define FLAG_DUPE_ALPHA "Wrong symbol OR duplicate in -C flag!"
#define FLAG_INVALID_VAL "Invalid value for flag: "
#define FLAG_INVALID_PROBS "Wrong use of [-p] flag! Use -help flag to see how to use it!"
#define FLAG_INVALID_RANGE "Invalid range! "
#define FLAG_UNEXPECTED_RANGE "You can't use -m1 or -m2, with -n OR just duplicate -n flag!"
#define FLAG_UNEXPECTED_ALPHA "You can't use -a and -C together OR duplicate this flags!"

#define CHECK_FLAG_1(str, symbol) (str[0] == '-' && str[1] == symbol)
#define CHECK_FLAG_2(str, flag) (str[0] == '-' && str[1] == flag[0] && str[2] == flag[1])

#define throw(msg, reserved, err_code) {printf("\nError: %s (%s)\n", msg, reserved); return err_code;} 

/*
набор символов (указывается один или несколько символов из множества {a, A, D, S}),
a - маленькие латинские символы,
A - большие латинские символы, 
D - цифры,
S - спецсимволы.
*/

#include <math.h>

NEW PassGenOptions* CreateEmptyGenOptions()
{
    PassGenOptions* options = calloc(1, sizeof(PassGenOptions));
    if (options == 0) return 0;
    options->minPasswordLength = -1;
    options->maxPasswordLength = -1;
    options->customAlphabet = 0;
    options->customAlphabetSize = 0;
    options->isAlphabetCustom = -1;
    options->passwordsCount = 1;
    options->letterProbabilities = 0;

    return options;
}

int initLetterProbabilities(char* format, OUT PassGenOptions* options)
{
    if (format == 0) return -1;
    if (format[0] == 0) return -1;
    LettersProbabilities* probs = calloc(1, sizeof(LettersProbabilities));
    if (probs == 0) return -1;
    options->letterProbabilities = probs;
    probs->probabilities = calloc(256, sizeof(char));
    if (probs->probabilities == 0) return -1;
    char token = '\0';
    int i = 0;
    while(format[i] != '\0') {
        token = format[i];
        if (format[i + 1] != '-') return -1;
        else i+=2;
        if (format[i] == '\0') return -1;
        char numBuf[20] = { 0 };
        int numPos = 0;
        while (format[i] != ',') {
            if (format[i] == '\0') break;
            if (format[i] < '0' || format[i] > '9') return -1;
            numBuf[numPos] = format[i];
            numPos++;
            i++;
        }
        int value = atoi(numBuf);
        probs->probabilities[token] = max(min(value, 100), 1);
        if (format[i] == '\0') break;
        i++;
    }
    return 0;
}

static int distributeProbabilities(char* alphabet, int alphaSize, LettersProbabilities* probs) {
    if (probs == 0) return 0;
    int remainingCount = 0;
    int remainingChance = 100;
    for (int i = 0; i < alphaSize; i++) {
        int p = probs->probabilities[alphabet[i]];
        if (p > 0) {
            probs->alphabetData[alphabet[i]] = p - 1;
            remainingChance -= p - 1;
        }
        else {
            remainingCount++;
            probs->alphabetData[alphabet[i]] = -1;
        }
    }
    if (remainingChance < 0) {
        probs->distributedChacne = 0;
        return -1;
    }
    if(remainingCount == 0) remainingCount = 1;
    probs->distributedChacne = remainingChance / remainingCount;
    return 0;
}

int randInt(int min, int max)
{
    int r = rand() ^ (rand() << 16);
    if (max < min || max == 0) return 0;
    return ((r % (max - min + 1)) + min);
}

static char getRandomChar(IN char* charArray, int charArrayLength, IN PassGenOptions* options) {

    LettersProbabilities* probs = options->letterProbabilities;

    if (probs == 0 || options->isAlphabetCustom == 0) {
        return charArray[randInt(0, charArrayLength - 1)];
    }
    
    char def = charArray[0];
    
    for (int i = 0; i < charArrayLength; i++) {
        char t = charArray[i];
        if (probs->alphabetData[t] > 0) {
            def = t;
            if (probs->alphabetData[t] >= randInt(0, 100)) return t;
        }
        if (probs->alphabetData[t] == -1 && probs->distributedChacne > 0) {
            def = t;
            if(probs->distributedChacne >= randInt(0, 100)) return t;
        }
    }
    return def;
}

static int getRandomSet(int defaultIndex, int setsCount ,IN PassGenOptions* options) {

    LettersProbabilities* probs = options->letterProbabilities;

    if (probs != 0) {
        for (int i = 0; i < ALPHABET_SETS_COUNT; i++) {
            int randValue = randInt(0, 100);
            if (options->isLatinLowerUsed && probs->alphabetData['a'] >= randValue) 
                return 0;
            if (options->isLatinUpperUsed && probs->alphabetData['A'] >= randValue) 
                return 1;
            if (options->isDigitsUsed     && probs->alphabetData['D'] >= randValue)
                return 2;
            if (options->isSpecialUsed    && probs->alphabetData['S'] >= randValue) 
                return 3;
        }
        return defaultIndex;
    }

    char* packed = &options->isLatinLowerUsed;
    int alpha_index = -1;
    for (int i = 0; i < ALPHABET_SETS_COUNT; i++) {
        if (packed[i] && (rand() % setsCount) && i != defaultIndex) {
            alpha_index = i;
            defaultIndex = i;
            break;
        }
    }
    if (alpha_index == -1) alpha_index = defaultIndex;
    return alpha_index;
}

NEW char* generatePassword(IN PassGenOptions* options)
{

    char** alpha_bucket[4] = { alpha_a_z, alpha_A_Z, alpha_0_9, alpha_S };
    char** alpha_sizes[4] = { 26, 26, 10, 30};
    if (options->maxPasswordLength < 0 
        || options->minPasswordLength < 0
        || options->minPasswordLength > options->maxPasswordLength
        ) {
        throw("Please provide password length or VALID range","for example: -n 10", 0);
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
    char* packed = &options->isLatinLowerUsed;
    int default_index = -2, packed_c = 0;
    for (int i = 0; i < ALPHABET_SETS_COUNT; i++) {
        if (packed[i]) default_index = (packed_c++, i);
    }
    if (options->isAlphabetCustom) {
        if (distributeProbabilities(alpha, alpha_size, options->letterProbabilities) != 0) {
            throw("Sum of probilities is overflow than 100!", "", 0);
        };
    }
    for (int i = 0; i < len; i++) {
        if (!options->isAlphabetCustom) {
            int alpha_index = getRandomSet(default_index, packed_c, options);
            alpha = alpha_bucket[alpha_index];
            alpha_size = alpha_sizes[alpha_index];
        }
        result[i] = getRandomChar(alpha, alpha_size, options);
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

static int handlePasswordLength(
    int args_c, IN char** argv, OUT PassGenOptions* options, int i
) {
    if (CHECK_FLAG_2(argv[i], "m1")) {
        if (options->minPasswordLength != -1) {
            throw(FLAG_DUPE, "-m1", -2);
        }
        int tmp = atoi(argv[i + 1]);
        if (tmp < 1 || tmp > MAX_PASSWORD_SIZE) {
            throw(FLAG_INVALID_VAL, "-m1", -3);
        }
        options->minPasswordLength = tmp;
    }
    if (CHECK_FLAG_2(argv[i], "-m2")) {
        if (options->maxPasswordLength != -1) {
            throw(FLAG_DUPE, "-m2", -2);
        }
        int tmp = atoi(argv[i + 1]);
        if (tmp < 1 || tmp > MAX_PASSWORD_SIZE) {
            throw(FLAG_INVALID_VAL, "-m2", -3);
        }
        options->maxPasswordLength = tmp;
    }
    if (CHECK_FLAG_1(argv[i], 'n')) {
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
    int args_c, IN char** argv, OUT PassGenOptions* options, int i, char* isCustom
) {
    if (CHECK_FLAG_1(argv[i], 'a')) {
        if (options->isAlphabetCustom != -1) {
            throw(FLAG_UNEXPECTED_ALPHA, "", -2);
        }
        options->isAlphabetCustom = 1;
        options->customAlphabet = CopyString(argv[i + 1], &options->customAlphabetSize);
        if (isCustom != 0) *isCustom = 1;
    }
    if (CHECK_FLAG_1(argv[i], 'C')) { //aADs = 
        if (options->isAlphabetCustom != -1) {
            throw(FLAG_UNEXPECTED_ALPHA, "", -2);
        }
        options->isAlphabetCustom = 0;
        int j = 0;
        int* packed = (int*)(&options->isLatinLowerUsed);
        while(argv[i + 1][j] != '\0') {
            if (argv[i + 1][j] == 'a') options->isLatinLowerUsed++;
            if (argv[i + 1][j] == 'A') options->isLatinUpperUsed++;
            if (argv[i + 1][j] == 'D') options->isDigitsUsed++;
            if (argv[i + 1][j] == 'S') options->isSpecialUsed++;
            int check = options->isLatinLowerUsed ^ options->isLatinUpperUsed ^
                options->isDigitsUsed ^ options->isSpecialUsed;
            if (check == 2 || *packed == 0) {
                throw(FLAG_DUPE_ALPHA, "", -3);
            };
            j++;
        }
    }
    return 0;
}

int readFlags(int args_c, IN char** argv, OUT PassGenOptions* options)
{
    *((int*)(&options->isLatinLowerUsed)) = 0;
    for (int i = 0; i < args_c; i++) {
        char* ref = argv[i];
        if (strcmp(argv[i], "-help") == 0) {
            showHelpAndExit();
        }
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
        int status = 1;
        char isCustomAlphabet = 0;
        if (handlePasswordLength(args_c, argv, options, i) != 0) status = 0;
        if (handlePasswordAlphabet(args_c, argv, options, i, &isCustomAlphabet) != 0) status = 0;
        if (isCustomAlphabet) { i++; continue; };
        if (CHECK_FLAG_1(argv[i], 'c')) {
            int tmp = atoi(argv[i + 1]);
            if (tmp < 1) throw(FLAG_INVALID_VAL, "-c", 5);
            options->passwordsCount = tmp;
        }
        if (CHECK_FLAG_1(argv[i], 'p')) {
            if (initLetterProbabilities(argv[i + 1], options) != 0) {
                throw(FLAG_INVALID_PROBS, "", 6);
            };
        }
        if (status == 0) return 1;
    }
    return 0;
}

void showHelpAndExit()
{
    printf("\nHelp(flags):\n");
    printf("\n\t-m1\tmin length of password\n");
    printf("\n\t-m2\tmax length of password\n");
    printf("\n\t-n\tfixed length of password\n");
    printf("\n\t-c\tcount of generated passwords\n");
    printf("\n\t-a\talphabet (set of symbols)\t\tUse: -a abc123Example \n");
    printf("\n\t-C\tchoose standart alphabet set\t\tUse: -C [aADS]");
    printf("\n\t\t[a] - a..z");
    printf("\n\t\t[A] - A..Z");
    printf("\n\t\t[D] - 0..9");
    printf("\n\t\t[S] - extra symbols ([]{}.,?!...)\n");
    printf("\n\t-p\tprobability of used chars\t\tUse: -p a-30,2-40,Z-10 ([char]-[chance])");
    printf("\n\t\tfor -C it sets probability for one of group[aADS], e.g.: -p a-30,A-20,D-50,S-0");
    printf("\n\t\tfor -a it sets probability for each symbol in aplhabet");
    printf("\n");
    exit(0);
}
