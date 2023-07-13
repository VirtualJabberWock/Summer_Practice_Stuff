/*
Пример использования цифровой подписи ГОСТ Р 34.10-2001.

Маткин Илья Александрович 18.03.2015
*/


#include <stdio.h>
#include <time.h>
#include <string.h>
#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <openssl/engine.h>
#include "gost_lcl.h"

//----------------------------------------

//#define _USE_DLL


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

ENGINE * LoadEngineById (char *engineName) {

ENGINE *engine = NULL;

    ENGINE_load_gost();
    engine = ENGINE_by_id (engineName);
    if (!engine) {
        printf("Filed to get structural reference to engine\n");
        }
    if (!ENGINE_init (engine)) {
        ENGINE_free (engine);
        printf ("Failed to get functional reference to engine\n");
        }

    ENGINE_set_default (engine, ENGINE_METHOD_ALL);
    OpenSSL_add_all_algorithms();

    return engine;
}


EVP_PKEY * GenerateKeys (ENGINE *engine) {

EVP_PKEY *pkey = EVP_PKEY_new();
EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id (NID_id_GostR3410_2001, engine);

    EVP_PKEY_paramgen_init (ctx);

    EVP_PKEY_CTX_ctrl (ctx,
                       NID_id_GostR3410_2001,
                       EVP_PKEY_OP_PARAMGEN,
                       EVP_PKEY_CTRL_GOST_PARAMSET,
                       NID_id_GostR3410_2001_CryptoPro_A_ParamSet,
                       NULL);

    EVP_PKEY_keygen_init (ctx);

    EVP_PKEY_keygen (ctx, &pkey);

    EVP_PKEY_CTX_free (ctx);

    return pkey;
}


int main (unsigned int argc, char *argv[], char *evnp[]) {

ENGINE *engine = NULL;
EVP_PKEY_CTX *ctx = NULL;
EVP_PKEY *pkey = NULL;
unsigned char msg[] = "this is a test message";
unsigned char hash[32];
unsigned char signature[0x1000];
size_t siglen = 0;
int status = 0;
int result;

    SHA256 (msg, sizeof(msg), &hash[0]);
    
    engine = LoadEngineById ("gost");
    pkey = GenerateKeys (engine);
    ctx = EVP_PKEY_CTX_new (pkey, engine);

    if (ctx == NULL) {
        printf("Failed to create context\n");
        return -1;
        }

    status = EVP_PKEY_sign_init (ctx);
    if (status != 1) {
        printf("Failed to init signing context\n");
        return -1;
        }

    status = EVP_PKEY_sign (ctx, NULL, &siglen, &hash[0], 32);
    if (status != 1) {
        printf("Failed to get signature length\n");
        return -1;
        }

    status = EVP_PKEY_sign (ctx, &signature[0], &siglen, &hash[0], 32);
    if (status != 1) {
        printf("Failed to sign a message\n");
        return -1;
        }

    EVP_PKEY_verify_init (ctx);

    result = EVP_PKEY_verify (ctx, &signature[0], siglen, &hash[0], 32);
    printf("%s\n", result ? "SUCCESS" : "FAILURE");

    hash[0]^=1;
    result = EVP_PKEY_verify (ctx, &signature[0], siglen, &hash[0], 32);
    printf("%s\n", result ? "SUCCESS" : "FAILURE");
    
    ENGINE_cleanup();

    return 0;
}
