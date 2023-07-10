#include "Kludges.h"

#if _IS_LOCALE_PROTECT_ACTIVE
#include <locale.h>
#endif

#if _WIN32
#include <Windows.h>
#else

#endif


#include <stdio.h>

void initLocaleProtect() {

#if _WIN32
	wchar_t locale[50];
	GetSystemDefaultLocaleName(&locale, 50);
	_wsetlocale(LC_ALL, locale);
#else
	char* locale = setlocale(LC_CTYPE, NULL);
	setlocale(LC_ALL, locale);
#endif
	
}

void initKludges()
{
#if _IS_LOCALE_PROTECT_ACTIVE
	initLocaleProtect();
#else
	printf("Warning! Not a standart ASCII, is not supported!\n\n");
#endif
}
