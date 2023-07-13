/*
Пример использования RSA.

Маткин Илья Александрович 18.03.2015
*/


#include <stdio.h>
#include <time.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <malloc.h>

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/pem.h>

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


clock_t start;
clock_t end;

#define calculate_time(start_time, end_time, action) \
    start_time = clock();\
    (action);\
    end_time = clock();\
    printf ("%f\n", (double)(end_time-start_time)/CLK_TCK);


//----------------------------------------


EVP_PKEY * GenerateRSAKeys (size_t bitsCount) {

EVP_PKEY *pkey = EVP_PKEY_new();
EVP_PKEY_CTX *ctx;

    ctx = EVP_PKEY_CTX_new_id (EVP_PKEY_RSA, NULL);

    EVP_PKEY_keygen_init (ctx);

    EVP_PKEY_CTX_set_rsa_keygen_bits (ctx, bitsCount);

    EVP_PKEY_keygen (ctx, &pkey);

    EVP_PKEY_CTX_free (ctx);

    return pkey;
}



void RsaEncryptFile (const char *fileName, const char *encryptFileName, EVP_PKEY_CTX *ctx) {

byte *buf;
unsigned int fileSize;
byte *encryptBuf;
size_t encryptBufSize;
int fd;

    fd = _open (fileName, _O_BINARY | _O_RDONLY);
    if (fd == -1) {
        return;
        }
    fileSize = _lseek (fd, 0, SEEK_END);
    _lseek (fd, 0, SEEK_SET);

    buf = (unsigned char*) malloc (fileSize);
    _read (fd, buf, fileSize);
    _close (fd);

    fd = _open (encryptFileName, _O_BINARY | _O_WRONLY | _O_CREAT | _O_TRUNC, 0666);
    if (fd == -1) {
        free (buf);
        return;
        }

    EVP_PKEY_encrypt_init (ctx);
    EVP_PKEY_CTX_set_rsa_padding (ctx, RSA_PKCS1_OAEP_PADDING);

    EVP_PKEY_encrypt (ctx, NULL, &encryptBufSize, buf, fileSize);
    encryptBuf = OPENSSL_malloc (encryptBufSize);
    EVP_PKEY_encrypt (ctx, encryptBuf, &encryptBufSize, buf, fileSize);

    _write (fd, encryptBuf, encryptBufSize);
    _close (fd);

    free (buf);
    OPENSSL_free (encryptBuf);
    return;
}


void RsaDecryptFile (const char *encryptFileName, const char *decryptFileName, EVP_PKEY_CTX *ctx) {

byte *encryptBuf;
size_t encryptBufSize;
byte *decryptBuf;
size_t decryptBufSize;
int fd;

    fd = _open (encryptFileName, _O_BINARY | _O_RDONLY);
    if (fd == -1) {
        return;
        }
    encryptBufSize = _lseek (fd, 0, SEEK_END);
    _lseek (fd, 0, SEEK_SET);

    encryptBuf = (unsigned char*) malloc (encryptBufSize);
    _read (fd, encryptBuf, encryptBufSize);
    _close (fd);

    fd = _open (decryptFileName, _O_BINARY | _O_WRONLY | _O_CREAT | _O_TRUNC, 0666);
    if (fd == -1) {
        free (encryptBuf);
        return;
        }

    EVP_PKEY_decrypt_init (ctx);
    EVP_PKEY_CTX_set_rsa_padding (ctx, RSA_PKCS1_OAEP_PADDING);

    EVP_PKEY_decrypt (ctx, NULL, &decryptBufSize, encryptBuf, encryptBufSize);
    decryptBuf = OPENSSL_malloc (decryptBufSize);
    EVP_PKEY_decrypt (ctx, decryptBuf, &decryptBufSize, encryptBuf, encryptBufSize);
    
    _write (fd, decryptBuf, decryptBufSize);
    _close (fd);

    free (encryptBuf);
    OPENSSL_free (decryptBuf);
    return;
}


int main (unsigned int argc, char *argv[], char *evnp[]) {

ENGINE *engine = NULL;
EVP_PKEY_CTX *ctx = NULL;
EVP_PKEY *pkey = NULL;
unsigned char msg[] = "this is a test message";
unsigned char digest[SHA256_DIGEST_LENGTH];
unsigned char signature[4096];
size_t siglen = 0;
int status = 0;
int result;
BIO *keyFile;

    OpenSSL_add_all_algorithms();

    SHA256 (msg, sizeof(msg), digest);
    
    engine = ENGINE_get_default_RSA();
    pkey = GenerateRSAKeys (2048);
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

    status = EVP_PKEY_sign (ctx, NULL, &siglen, digest, SHA256_DIGEST_LENGTH);
    if (status != 1) {
        printf("Failed to get signature length\n");
        return -1;
        }

    status = EVP_PKEY_sign (ctx, &signature[0], &siglen, digest, SHA256_DIGEST_LENGTH);
    if (status != 1) {
        printf("Failed to sign a message\n");
        return -1;
        }

    EVP_PKEY_verify_init (ctx);
    digest[5]^=1;
    result = EVP_PKEY_verify (ctx, &signature[0], siglen, digest, SHA256_DIGEST_LENGTH);
    printf("%s\n", result ? "SUCCESS" : "FAILURE");
    
    digest[5]^=1;
    result = EVP_PKEY_verify (ctx, &signature[0], siglen, digest, SHA256_DIGEST_LENGTH);
    printf("%s\n", result ? "SUCCESS" : "FAILURE");

    // сохраняем приватный ключ и шифруем файл
    keyFile = BIO_new_file ("key.pem", "wb");
    PEM_write_bio_PKCS8PrivateKey (keyFile, pkey, EVP_des_ede3_cbc(), NULL, 0, 0, "keypass");
    BIO_free (keyFile);
    RsaEncryptFile ("test.txt", "test.txt.enc", ctx);
    EVP_PKEY_CTX_free (ctx);

    // загружаем приватный ключ и расшифровываем файл
    keyFile = BIO_new_file ("key.pem", "rb");
    pkey = PEM_read_bio_PrivateKey (keyFile, NULL, 0, "keypass");
    BIO_free (keyFile);
    ctx = EVP_PKEY_CTX_new (pkey, engine);
    RsaDecryptFile ("test.txt.enc", "test.txt.dec", ctx);
    EVP_PKEY_CTX_free (ctx);

    ENGINE_cleanup();

    return 0;
}
