#include "string_compare.h"
#pragma once

#include <string.h>

int Compare_StrBasic(char* str1, char* str2, int maxLength)
{
	int i = 0;
	int len2 = 0;
	if (maxLength == -1) len2 = strlen(str2);
	if (maxLength >= 0) len2 = strnlen_s(str2, maxLength);
	while (str1[i] != 0) {
		if (str1[i] > str2[i]) {
			return 1;
		}
		if (str1[i] < str2[i]) {
			return -1;
		}
		i++;
	}
	if (i == len2) {
		return 0;
	}
	if (i > len2) {
		return 1;
	}
	return -1;
}

int Compare_StrDescending(char* str1, char* str2, int maxLength)
{
	int i = 0;
	int len2 = 0;
	if(maxLength == -1) len2 = strlen(str2);
	if (maxLength >= 0) len2 = strnlen_s(str2, maxLength);
	while (str1[i] != 0) {
		if (str1[i] < str2[i]) {
			return 1;
		}
		if (str1[i] > str2[i]) {
			return -1;
		}
		i++;
	}
	if (i == len2) {
		return 0;
	}
	if (i < len2) {
		return 1;
	}
	return -1;
}

int Compare_StrCaseInsensetive(char* str1, char* str2, int maxLength)
{
	int i = 0;
	int len2 = 0;
	if (maxLength == -1) len2 = strlen(str2);
	if (maxLength >= 0) len2 = strnlen_s(str2, maxLength);
	while (str1[i] != 0) {
		char c1 = str1[i];
		char c2 = str2[i];
		if (c1 >= 'a' && c1 <= 'z') {
			c1 = c1 - ' ';
		}
		if (c2 >= 'a' && c2 <= 'z') {
			c2 = c2 - ' ';
		}
		if (c1 > c2) return 1;
		if (c1 < c2) return -1;
		i++;
	}
	if (i == len2) {
		return 0;
	}
	if (i > len2) {
		return 1;
	}
	return -1;
}

int Compare_StrByLength(char* str1, char* str2, int maxLength)
{
	int l2 = 0;
	if (maxLength == -1) l2 = strlen(str2);
	if (maxLength >= 0) l2 = strnlen_s(str2, maxLength);
	int l1 = 0;
	if (maxLength == -1) l1 = strlen(str1);
	if (maxLength >= 0) l1 = strnlen_s(str1, maxLength);
	if (l1 > l2) return 1;
	if (l1 < l2) return -1;
	return 0;
}
