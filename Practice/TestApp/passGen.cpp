#include "passGen.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

typedef enum alphaCount
{
    LOWER = 0,
    UPPER,
    DIGIT,
    SPECIAL
}alphaCount;


char** getPasswords(int argc, char** argv, int* errCode)
{
    int len = 0; int minLen = 0; int maxLen = 0;
    int nFlag = 0; int mFlag = 0; int MFlag = 0; int cFlag = 0; int aFlag = 0; int bigCFlag = 0;
    int countPasswords = 1;
    char* alpha = NULL;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-m1", argv[i]) == 0)
        {
            if (i != argc - 1)
            {
                if (strlen(argv[i + 1]) != 0  && atoll(argv[i + 1]) != 0 && mFlag == 0 && nFlag == 0)
                {
                    minLen = atoll(argv[i + 1]);
                    mFlag = 1;
                    i++;
                }
                else
                {
                    *errCode = ERR_WITH_LENGTH;
                    return NULL;
                }
            }
            else
            {
                *errCode = ERR_WITH_LENGTH;
                return NULL;
            }
        }
        else if (strcmp("-m2", argv[i]) == 0)
        {
            if (i != argc - 1)
            {
                if (strlen(argv[i + 1]) != 0  && atoll(argv[i + 1]) != 0 && MFlag == 0 && nFlag == 0)
                {
                    maxLen = atoll(argv[i + 1]);
                    MFlag = 1;
                    i++;
                }
                else
                {
                    *errCode = ERR_WITH_LENGTH;
                    return NULL;
                }
            }
            else
            {
                *errCode = ERR_WITH_LENGTH;
                return NULL;
            }
        }
        else if (strcmp("-n", argv[i]) == 0)
        {
            if (i != argc - 1)
            {
                if (strlen(argv[i + 1]) != 0 && atoll(argv[i + 1]) != 0 && MFlag == 0 && mFlag == 0)
                {
                    len = atoll(argv[i + 1]);
                    if (len <= 0)
                    {
                        *errCode = ERR_WITH_LENGTH;
                        return NULL;
                    }
                    nFlag = 1;
                    i++;
                }
                else
                {
                    *errCode = ERR_WITH_LENGTH;
                    return NULL;
                }
            }
            else
            {
                *errCode = ERR_WITH_LENGTH;
                return NULL;
            }
        }
        else if (strcmp("-c", argv[i]) == 0)
        {
            if (i != argc - 1)
            {
                if (strlen(argv[i + 1]) != 0 && atoll(argv[i + 1]) != 0 && cFlag == 0)
                {
                    countPasswords = atoi(argv[i + 1]);
                    cFlag = 1;
                    i++;
                }
                else
                {
                    *errCode = ERR_WITH_PASSCOUNT;
                    return NULL;
                }
            }
            else
            {
                *errCode = ERR_WITH_PASSCOUNT;
                return NULL;
            }
        }
        else if (strcmp("-a", argv[i]) == 0)
        {
            if (i != argc - 1 && aFlag == 0 && bigCFlag == 0)
            {
                if (aFlag == 0 && bigCFlag == 0)
                {
                    alpha = argv[i + 1];
                    i++;
                    aFlag = 1;
                }
                else
                {
                    *errCode = ERR_WITH_ALPHA;
                    return NULL;
                }
            }
            else
            {
                *errCode = ERR_WITH_ALPHA;
                return NULL;
            }
        }
        else if (strcmp("-C", argv[i]) == 0)
        {
            if (i != argc - 1 && aFlag == 0 && bigCFlag == 0)
            {
                alpha = getAlphabet(argv[i + 1]);
                i++;
                bigCFlag = 1;
            }
            else 
            {
                *errCode = ERR_WITH_ALPHA;
                return NULL;
            }
        }
    }
    if (MFlag == 1 && maxLen > (INT_MAX))
    {
        *errCode = ERR_WITH_LENGTH;
        return NULL;
    }
    if (mFlag == 1 && minLen >= INT_MAX)
    {
        *errCode = ERR_WITH_LENGTH;
        return NULL;
    }
    if (minLen == 0 && mFlag == 1)
    {
        *errCode = ERR_WITH_LENGTH;
        return NULL;
    }
    if (minLen < 0 || maxLen < 0)
    {
        *errCode = ERR_WITH_LENGTH;
        return NULL;
    }
    if (mFlag != 0 && MFlag == 0)
    {
        *errCode = ERR_WITH_LENGTH;
        return NULL;
    }
    
    if (minLen > maxLen || len < 0)
    {
        *errCode = ERR_WITH_LENGTH;
        return NULL;
    }
    if (nFlag == 1 && len <= 0)
    {
        *errCode = ERR_WITH_LENGTH;
        return NULL;
    }
    printf("FINAL\nminLen = %d,\nmaxLen = %d,\nlen = %d,\npassCount = %d\nalphabet = %s\n", minLen, maxLen, len, countPasswords, alpha);
    return NULL;     
}

char* generatePassword(int minLen, int maxLen, int len, char* alphabet)
{
    int size;
    if (len != 0)
    {
        size = len;
    }
    else
    {
        if (minLen == maxLen)
        {
            size = maxLen;
        }
        else
        {
            size = rand() % (maxLen - minLen + 1) + minLen;
        }
    }
    char* pass = (char*)malloc(sizeof(char) * (size + 1));
    if (pass == NULL)
    {
        return NULL;
    }
    int alphaLen = strlen(alphabet);
    for (int i = 0; i < size; i++)
    {
        int s = rand();
        pass[i] = alphabet[s % alphaLen];
    }
    pass[size] = '\0';
    return pass;
}

char* getAlphabet(char* potenAlpha)
{
    int len = strlen(potenAlpha);
    if (len > 4) return NULL;
    int masCount[4] = {0};
    for (int i = 0; i < len; i++)
    {
        if (potenAlpha[i] == 'a') masCount[LOWER]++;
        else if (potenAlpha[i] == 'A') masCount[UPPER]++;
        else if (potenAlpha[i] == 'D') masCount[DIGIT]++;
        else if (potenAlpha[i] == 'S') masCount[SPECIAL]++;
        else return NULL;
    }
    for (int i = 0; i < 4; i++)
    {
        if (masCount[i] > 1) return NULL;
    }
    char upperCase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char lowerCase[] = "abcdefghijklmnopqrstuvwxyz";
    char digits[] = "0123456789";
    char specialSymbols[] = "!@#$\\%^&*()\"№;:?_-=+~|/[]{}\' ";
    char rawAlphabet[100] = "";
    if (masCount[LOWER]) strcat_s(rawAlphabet, 27, lowerCase);
    if (masCount[UPPER]) strcat_s(rawAlphabet, 27, upperCase);
    if (masCount[DIGIT]) strcat_s(rawAlphabet, 11, digits);
    if (masCount[SPECIAL]) strcat_s(rawAlphabet, 30, specialSymbols);
    char* alphabet = (char*)malloc(sizeof(char) * (strlen(rawAlphabet) + 1));
    for (int i = 0; i < strlen(rawAlphabet); i++)
        alphabet[i] = rawAlphabet[i];
    alphabet[strlen(rawAlphabet)] = '\0';
    return alphabet;
}

char* concat(char* f, char* s)
{
    int len1 = strlen(f);
    int len2 = strlen(f);                      
    char *result = (char*)malloc(len1 + len2 + 1);
    if (!result) 
    {
        return NULL;
    }
    memcpy(result, f, len1);
    memcpy(result + len1, s, len2 + 1);   
    result[len1 + len2] = '\0'; 
    return result;
}