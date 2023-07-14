#pragma once

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

/*

Задания.

1. Реализовать сортировку строк файла.

Условия для оценки 3

Завести статический двумерный массив из STRINGS_COUNT строк и MAX_STRING_SIZE столбцов
и динамический массив из STRINGS_COUNT строк. Массивы считаем параллельно проиндексированными,
т.е. в элементах с одинаковыми индексами расположены соответствующие строки.
Другие массивы для хранения строк использовать нельзя.
Считать из файла не более STRINGS_COUNT строк.
Если строка не больше MAX_STRING_SIZE расположить её в строке статического двумерного массива. 
Если больше - первые MAX_STRING_SIZE символов расположить в строке статического двумерного массива,
а остальные в соответствующей строке динамического массива.
В итоге строки (в зависимости от длины) будут частично храниться в статическом массиве, частично в динамическом.
Реализовать сортировку строк, хранящихся описанным образом.
Во время сортировки необходимо согласованным образом переставлять соответствующие части строк в двух массивах.
В функцию сортировки передавать указатель на функцию сравнения строк.
Предложить и реализовать не менее четырёх разных функций сравнения строк 
(среди них обязательно функции сравнения в лексикографическом порядке с учетом и без учета регистра).
Для тестирования реализовать генерацию файла строк (не менее STRINGS_COUNT строк). Строки по длине должны быть меньше, равны и больше MAX_STRING_SIZE.

Шаблон для задания в каталоге template\1.


Условия для оценки 4
Реализовать быструю сортировку.

*/

#define STRINGS_COUNT   500
#define MAX_STRING_SIZE 100

//const char* printable = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-=+[]\\/*(){};:,.?!<>^%$#@~\'\"";
const char* printable = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* __empty__ = "";

void GenerateRandomString(char* str, int size) {

    for (int i = 0; i < size; i++) {
        str[i] = printable[rand() % 62];
    }
}


void GenerateRandomStrings(char staticArray[][MAX_STRING_SIZE], char** Array) {

    for (int i = 0; i < STRINGS_COUNT; i++) {

        int size = rand() % (MAX_STRING_SIZE * 3);
        if (size == 0) size = 1;

        char* word = calloc(size + 1, sizeof(char));
        if (word == 0) {
            printf("Memory allocation error!");
            exit(-1);
            return;
        }
        word[size] = 0;

        GenerateRandomString(word, size);

        int left = size;
      
        for (int j = 0; j < MAX_STRING_SIZE; j++) {
            staticArray[i][j] = word[j];
            left -= 1;
            if (left == 0) break;
        }
        if (left <= 0) {
            Array[i] = 0;
        }
        else {
            Array[i] = word + MAX_STRING_SIZE;
        }
    }

}

void PrintStrings(char staticStrings[][MAX_STRING_SIZE], char** dynamicStrings, FILE* stream) {

    for (int i = 0; i < STRINGS_COUNT; i++) {

        for (int j = 0; j < MAX_STRING_SIZE; j++) 
        {
            char c = staticStrings[i][j];
            if (c == 0) break;
            fwrite(&c, 1, 1, stream);
            ("%c", c);
        }

        if (dynamicStrings[i] != 0) {
            fprintf(stream, "%s", dynamicStrings[i]);
        }

        fprintf(stream, "\n");
    }
}


typedef int (*compareStringFunc) (char*, char*, int maxLen);

int CompareStringWrap
(
    char staticStrings[][MAX_STRING_SIZE], char** dynamicStrings,
    int i, int j, compareStringFunc cmpFunc
) 
{
    if (staticStrings[i] == 0) return 0;
    if (staticStrings[j] == 0) return 0;

    int sCmp = cmpFunc(staticStrings[i], staticStrings[j], MAX_STRING_SIZE);
    if (sCmp != 0) {
        return sCmp;
    }

    if (dynamicStrings[i] != 0 && dynamicStrings[j] == 0) return 1;
    if (dynamicStrings[i] == 0 && dynamicStrings[j] != 0) return -1;
    if (dynamicStrings[i] == 0 && dynamicStrings[j] == 0) return 0;

    return cmpFunc(dynamicStrings[i], dynamicStrings[j], -1);
}

void swapStrings(char staticStrings[][MAX_STRING_SIZE], char** dynamicStrings, int i, int j) {

    char* temp = dynamicStrings[i];
    dynamicStrings[i] = dynamicStrings[j];
    dynamicStrings[j] = temp;
    
    char bufferTemp[MAX_STRING_SIZE] = { 0 };
    memcpy(bufferTemp, staticStrings + i , MAX_STRING_SIZE);
    memcpy(staticStrings + i, staticStrings + j, MAX_STRING_SIZE);
    memcpy(staticStrings + j, bufferTemp, MAX_STRING_SIZE);
}

int partition(char staticStrings[][MAX_STRING_SIZE], char** dynamicStrings, int low, int high, compareStringFunc compareStringsFunction)
{
    int pivotIndex = high;

    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        int cmpResult = CompareStringWrap(staticStrings, dynamicStrings, pivotIndex, j, compareStringsFunction);
        if (cmpResult > 0) {
            i++;
            swapStrings(staticStrings, dynamicStrings, i, j);
        }
    }
    swapStrings(staticStrings, dynamicStrings, i + 1, high);
    return (i + 1);
}

void SortStrings(char staticStrings[][MAX_STRING_SIZE], char** dynamicStrings, void* compareStringsFunction, int left, int right) {

    if (right < left) return;

    int pivotPtr = partition(staticStrings, dynamicStrings, left, right, compareStringsFunction);

    SortStrings(staticStrings, dynamicStrings, compareStringsFunction, left, pivotPtr - 1);
    SortStrings(staticStrings, dynamicStrings, compareStringsFunction, pivotPtr + 1, right);

}

#include "string_compare.h"

int main(int argc, char* argv[]) {

    FILE* output;

    fopen_s(&output,"output.txt", "w");
        
    char staticStrings[STRINGS_COUNT][MAX_STRING_SIZE] = { 0 };
    char** dynamicStrings;

    dynamicStrings = calloc(STRINGS_COUNT, sizeof(char*));

    GenerateRandomStrings(staticStrings, dynamicStrings);
    SortStrings(staticStrings, dynamicStrings, Compare_StrByLength, 0, STRINGS_COUNT - 1);
    PrintStrings(staticStrings, dynamicStrings, output);
 
    _fcloseall();

    return 0;
}