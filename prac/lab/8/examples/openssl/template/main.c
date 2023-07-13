/*
Шаблон проекта для библиотеки openssl.

Маткин Илья Александрович 11.05.2013
*/


#include <stdio.h>
#include <time.h>
#include <string.h>
#include <openssl/opensslv.h>
#include <openssl/crypto.h>

//----------------------------------------

#define _USE_DLL


#ifdef _WIN64

#ifdef _USE_DLL

#pragma comment( lib, "..\\lib_shared_64\\libeay32.lib" )
#pragma comment( lib, "..\\lib_shared_64\\ssleay32.lib" )

#else // _USE_DLL

#pragma comment( lib, "..\\lib_static_64\\libeay32.lib" )
#pragma comment( lib, "..\\lib_static_64\\ssleay32.lib" )

#endif // _USE_DLL

#else // _WIN64

#ifdef _USE_DLL

#pragma comment( lib, "..\\lib_shared_32\\libeay32.lib" )
#pragma comment( lib, "..\\lib_shared_32\\ssleay32.lib" )

#else // _USE_DLL

#pragma comment( lib, "..\\lib_static_32\\libeay32.lib" )
#pragma comment( lib, "..\\lib_static_32\\ssleay32.lib" )

#endif // _USE_DLL

#endif // _WIN64


//----------------------------------------


clock_t start;
clock_t end;

#define calculate_time(start_time, end_time, action) \
    start_time = clock();\
    (action);\
    end_time = clock();\
    printf ("%f\n", (double)(end_time-start_time)/CLK_TCK);


//----------------------------------------

int main(unsigned int argc,char *argv[]){

    
    puts (OPENSSL_VERSION_TEXT);
    puts (SSLeay_version (SSLEAY_VERSION));
    puts (SSLeay_version (SSLEAY_CFLAGS));
    puts (SSLeay_version (SSLEAY_BUILT_ON));
    puts (SSLeay_version (SSLEAY_PLATFORM));
    puts (SSLeay_version (SSLEAY_DIR));

    return 0;
}