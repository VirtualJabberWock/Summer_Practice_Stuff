/*
Пример использования больших числе из библиотеки openssl.

Маткин Илья Александрович 18.03.2015
*/


#include <stdio.h>
#include <time.h>
#include <string.h>
#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <openssl/bn.h>
#include <openssl/bio.h>

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

int main(unsigned int argc,char *argv[]){

    
BIGNUM *a = BN_new();
BIGNUM *b = NULL;
BIGNUM *c = BN_new();
BN_CTX *ctx = BN_CTX_new();
BIO *out;
 
    out = BIO_new_fd (fileno(stdout), BIO_NOCLOSE);
    
    BN_hex2bn (&a, "102030aabb");
    BN_dec2bn (&b, "1234567");
    BN_print (out, a);
    BIO_printf (out, "\nb = %s (0x%s)\n", BN_bn2dec (b), BN_bn2hex (b));

    BN_add (c, a, b);
    BIO_printf (out, "a+b = c = %s (0x%s)\n", BN_bn2dec (c), BN_bn2hex (c));
    BN_add_word (c, 1);
    BIO_printf (out, "++c = %s (0x%s)\n", BN_bn2dec (c), BN_bn2hex (c));

    BN_mul (c, a, b, ctx);
    BIO_printf (out, "a*b = c = %s (0x%s)\n", BN_bn2dec (c), BN_bn2hex (c));

    BN_rand (a, 1024, 1, 0);
    BIO_printf (out, "a = %s (0x%s)\n", BN_bn2dec (a), BN_bn2hex (a));
    
    BN_generate_prime_ex (a, 1024, 0, NULL, NULL, NULL);
    BIO_printf (out, "a = %s (0x%s)\n", BN_bn2dec (a), BN_bn2hex (a));
    if (BN_is_prime (a, 25, NULL, ctx, NULL)) {
        BIO_printf (out, "a is prime\n");
        }
    if (BN_is_prime (c, 25, NULL, ctx, NULL)) {
        BIO_printf (out, "c is prime\n");
        }

    BN_rand (b, 1024, 1, 0);
    BIO_printf (out, "b = %s (0x%s)\n", BN_bn2dec (b), BN_bn2hex (b));
    BN_mod_mul (c, b, b, a, ctx);
    BIO_printf (out, "b^2 mod a = c = %s (0x%s)\n", BN_bn2dec (c), BN_bn2hex (c));
    BN_mod_inverse (b, c, a, ctx);
    BIO_printf (out, "c^(-1) mod a = b = %s (0x%s)\n", BN_bn2dec (b), BN_bn2hex (b));
    BN_mod_mul (c, b, c, a, ctx);
    BIO_printf (out, "c*b mod a = c = %s (0x%s)\n", BN_bn2dec (c), BN_bn2hex (c));

    BN_free (a);
    BN_free (b);
    BIO_free(out);
    return 0;
}
