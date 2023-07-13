/*
Пример использования AES в режиме ECB
из библиотеки openssl.

Маткин Илья Александрович 11.05.2013
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <openssl/aes.h>
#include <openssl/blowfish.h>


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


int aes_ecb_encrypt_file (const char *in_file, const char *out_file, unsigned char *pass) {


int in, out;
unsigned char plaintext[AES_BLOCK_SIZE];
unsigned char ciphertext[AES_BLOCK_SIZE];
unsigned int size;
unsigned int i;
unsigned int file_size;

AES_KEY key;

    AES_set_encrypt_key (pass, 8 * (int)strlen (pass), &key);

    in = _open (in_file, _O_RDONLY | _O_BINARY);
    if (in == -1) {
        perror ("Error open input file: ");
        return 0;
        }
    file_size = _filelength (in);

    out = _open (out_file, _O_WRONLY | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
    if (out == -1) {
        perror ("Error open output file: ");
        _close (in);
        return 0;
        }

    while ( (size = _read (in, plaintext, AES_BLOCK_SIZE)) == AES_BLOCK_SIZE ) {
        AES_encrypt (plaintext, ciphertext, &key);
        _write (out, ciphertext, AES_BLOCK_SIZE);
        }

    if (size) {
        for (i = size; i < AES_BLOCK_SIZE; ++i) {
            plaintext[i] = 0;
            }
        AES_encrypt (plaintext, ciphertext, &key);
        _write (out, ciphertext, AES_BLOCK_SIZE);
        }

    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        plaintext[i] = 0;
        }
    *((unsigned int*)plaintext) = file_size;
    AES_encrypt (plaintext, ciphertext, &key);
    _write (out, ciphertext, AES_BLOCK_SIZE);

    _close (out);
    _close (in);

    return 1;
}



int aes_ecb_decrypt_file (const char *in_file, const char *out_file, unsigned char *pass) {


int in, out;
unsigned char plaintext[AES_BLOCK_SIZE];
unsigned char ciphertext[AES_BLOCK_SIZE];
unsigned int size;
unsigned int file_size;

AES_KEY key;

    AES_set_decrypt_key (pass, 8 * (int)strlen(pass), &key);

    in = _open (in_file, _O_RDONLY | _O_BINARY);
    if (in == -1) {
        perror ("Error open input file: ");
        return 0;
        }
    file_size = _filelength (in);

    out = _open (out_file, _O_WRONLY | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
    if (out == -1) {
        perror ("Error open output file: ");
        _close (in);
        return 0;
        }

    while ( (size = _read (in, ciphertext, AES_BLOCK_SIZE)) == AES_BLOCK_SIZE ) {
        AES_decrypt (ciphertext, plaintext, &key);
        _write (out, plaintext, AES_BLOCK_SIZE);
        }

    file_size = *((unsigned int*)plaintext);

    _chsize (out, file_size);

    _close (out);
    _close (in);

    return 1;
}


int bf_ecb_encrypt_file (const char *in_file, const char *out_file, unsigned char *pass) {


int in, out;
unsigned char plaintext[BF_BLOCK];
unsigned char ciphertext[BF_BLOCK];
unsigned int size;
unsigned int i;
unsigned int file_size;

BF_KEY key;

    BF_set_key (&key, (int)strlen (pass), pass);

    in = _open (in_file, _O_RDONLY | _O_BINARY);
    if (in == -1) {
        perror ("Error open input file: ");
        return 0;
        }
    file_size = _filelength (in);

    out = _open (out_file, _O_WRONLY | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
    if (out == -1) {
        perror ("Error open output file: ");
        _close (in);
        return 0;
        }

    while ( (size = _read (in, plaintext, BF_BLOCK)) == BF_BLOCK ) {
        //AES_encrypt (plaintext, ciphertext, &key);
        BF_ecb_encrypt (plaintext, ciphertext, &key, BF_ENCRYPT);
        _write (out, ciphertext, BF_BLOCK);
        }

    if (size) {
        for (i = size; i < BF_BLOCK; ++i) {
            plaintext[i] = 0;
            }
        //AES_encrypt (plaintext, ciphertext, &key);
        BF_ecb_encrypt (plaintext, ciphertext, &key, BF_ENCRYPT);
        _write (out, ciphertext, BF_BLOCK);
        }

    for (i = 0; i < BF_BLOCK; ++i) {
        plaintext[i] = 0;
        }
    *((unsigned int*)plaintext) = file_size;
    //AES_encrypt (plaintext, ciphertext, &key);
    BF_ecb_encrypt (plaintext, ciphertext, &key, BF_ENCRYPT);
    _write (out, ciphertext, BF_BLOCK);

    _close (out);
    _close (in);

    return 1;
}



int bf_ecb_decrypt_file (const char *in_file, const char *out_file, unsigned char *pass) {


int in, out;
unsigned char plaintext[BF_BLOCK];
unsigned char ciphertext[BF_BLOCK];
unsigned int size;
unsigned int file_size;

BF_KEY key;

    BF_set_key (&key, (int)strlen (pass), pass);

    in = _open (in_file, _O_RDONLY | _O_BINARY);
    if (in == -1) {
        perror ("Error open input file: ");
        return 0;
        }
    file_size = _filelength (in);

    out = _open (out_file, _O_WRONLY | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
    if (out == -1) {
        perror ("Error open output file: ");
        _close (in);
        return 0;
        }

    while ( (size = _read (in, ciphertext, BF_BLOCK)) == BF_BLOCK ) {
        //AES_decrypt (ciphertext, plaintext, &key);
        BF_ecb_encrypt (ciphertext, plaintext, &key, BF_DECRYPT);
        _write (out, plaintext, BF_BLOCK);
        }

    file_size = *((unsigned int*)plaintext);

    _chsize (out, file_size);

    _close (out);
    _close (in);

    return 1;
}



int main(unsigned int argc,char *argv[]){


    unsigned char *pass = "0123456789ABCDEF";

    //calculate_time (start, end, aes_ecb_encrypt_file ("test.txt", "test.enc", pass));
    //calculate_time (start, end, aes_ecb_decrypt_file ("test.enc", "test.dec.txt", pass));
    calculate_time (start, end, bf_ecb_encrypt_file ("test.txt", "test.enc", pass));
    calculate_time (start, end, bf_ecb_decrypt_file ("test.enc", "test.dec.txt", pass));

    return 0;
}
