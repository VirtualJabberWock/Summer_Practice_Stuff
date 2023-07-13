/*
Пример вычисления хешей в библиотеке openssl.

Маткин Илья Александрович 18.03.2015
*/


#include <stdio.h>
#include <time.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/evp.h>

//----------------------------------------


typedef unsigned char byte;

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


void md5_msg (const byte *msg, size_t msgSize, byte *digest) {

MD5_CTX ctx;

    MD5_Init (&ctx);

    MD5_Update (&ctx, msg, msgSize / 2);
    MD5_Update (&ctx, msg + msgSize / 2, msgSize - msgSize / 2);
    // MD5_Update (&ctx, msg, msgSize);

    MD5_Final (digest, &ctx);

    return 0;
}


#define FILE_BLOCK_SIZE 1024

size_t DigestFile (const char *fileName, byte *digest, const char *digestName) {

unsigned char block[FILE_BLOCK_SIZE];
unsigned int blockSize;
EVP_MD_CTX *mdctx;
const EVP_MD *md;
size_t digestSize;
int fd;

    mdctx = EVP_MD_CTX_create();
    // sha, sha1, mdc2, ripemd160, sha224, sha256, sha384, sha512, md2, md4, md5, dss1
    md = EVP_get_digestbyname (digestName);
    EVP_DigestInit_ex (mdctx, md, NULL);

    fd = _open (fileName, _O_BINARY);
    if (fd == -1) {
        return 0;
        }

    while ( (blockSize = _read (fd, block, FILE_BLOCK_SIZE)) == FILE_BLOCK_SIZE) {
        EVP_DigestUpdate (mdctx, block, blockSize);
        }
    EVP_DigestUpdate (mdctx, block, blockSize);

    EVP_DigestFinal_ex (mdctx, digest, &digestSize);

    _close (fd);

    return digestSize;
}



void PrintDigest (byte *hash, size_t size) {

size_t i;

    for (i = 0; i < size; ++i) {
        printf ("%02X", hash[i]);
        }

    return;
}



int main (unsigned int argc, char *argv[]) {

byte digest[EVP_MAX_MD_SIZE];
size_t i;
char *str = "abc";

    // делает алгоритмы хеширования доступными для поиска
    OpenSSL_add_all_digests();

    printf ("md5 (%s) = ", str);
    MD5 (str, strlen(str), digest);
    PrintDigest (digest, MD5_DIGEST_LENGTH);
    printf ("\n");

    printf ("sha256 (%s) = ", str);
    SHA256 (str, strlen(str), digest);
    PrintDigest (digest, SHA256_DIGEST_LENGTH);
    printf ("\n");


    if (argv[1]) {
        size_t digestSize;
        
        digestSize = DigestFile (argv[1], digest, "md5");
        if (digestSize) {
            printf ("md5 file %s = ", argv[1]);
            PrintDigest (digest, digestSize);
            printf ("\n");
            }

        digestSize = DigestFile (argv[1], digest, "sha256");
        if (digestSize) {
            printf ("md5 file %s = ", argv[1]);
            PrintDigest (digest, digestSize);
            printf ("\n");
            }
        }

    return 0;
}
