#pragma once
#include "Object.h"
#include "Functional.h"

#include <setjmp.h>

void _DebugObjectMethod(Object* obj, ...);

static int exit_if_error = 1;
static char* last_error = 0;
static unsigned int last_error_code = 0;
static int last_check = 0;
static int is_jump_context_on = 0;
extern jmp_buf debug_jump_buffer;

//machine (os)
static const unsigned int ERR_CANT_ALLOC_MEM = 0xdeadbeef;

//standart
static const unsigned int ERR_NULL_POINTER = 0xCCCCCCCC; // [CCCCCCCC]
static const unsigned int ERR_INVALID_CAST = 0xBADCA57; // BAD CAST
static const unsigned int ERR_NOT_IMPLEMENTED = 0xf150CCCC; // f is 0

//special
static const unsigned int ERR_BAD_NUMBER_FORMAT = 0xbad1234F; 

int panic(const char* err_msg, unsigned int err_code);
int panic_npe(const char* func_name, int line, const char* filename);
int panic_custom(const char* func_name, int line, const char* filename, const char* desc, int code);
int panic_cast(const char* to_type, const char* func_name, int line, const char* filename);

#define MEM_PANIC_EXCEPTION {panic("Can't alloc memory!", ERR_CANT_ALLOC_MEM); return 0;}
#define NULL_POINTER_EXCEPTION {panic_npe(__func__, __LINE__, __FILE__); return 0;}
#define NUMBER_FORMAT_EXCEPTION {panic_custom(__func__, __LINE__, __FILE__, "Number format exeption!", 0xbad1234F); return 0;}
#define INVALID_CAST_EXCEPTION(name) {panic_cast(name,__func__, __LINE__, __FILE__); return 0;}
#define throw
#define __pass {}

#define UNEXPECTED_PTR0 0xfdfdfdfd00000000
#define UNEXPECTED_PTR1 0xcccccccc00000000
#define UNEXPECTED_PTR2 0xcccccccccccccccc

int win_panic(const char* err_msg);
int unix_panic(const char* err_msg);

const char* DEBUG_GetLastError();
int DEBUG_GetLastErrorCode();
int _DEBUG_FreeErrorCode();

void _DEBUG_SetErrorCatchContext();
int _DEBUG_GetErrorStatusAndSetJump();
void _DEBUG_ReleaseErrorCatchContext(int status);

#define IMPORT_TRY_CATCH jmp_buf debug_jump_buffer; int _DONT_FORGET_IMPORT_TRY_CATCH = 0;

/* use DEBUG_GetLastError() and DEBUG_GetLastErrorCode() */
#define CATCH 
/* 
use DEBUG_GetLastError() and DEBUG_GetLastErrorCode()
for use this macro, type "IMPORT_TRY_CATCH" macro, in c file;
*/
#define TRY_TO_EXECUTE(__code__, catch_code) \
{\
	_DONT_FORGET_IMPORT_TRY_CATCH = 1; \
	_DEBUG_SetErrorCatchContext(); \
	int __isErrorOccured = setjmp(debug_jump_buffer); \
	if (__isErrorOccured) { \
		if(DEBUG_GetLastError() == 0) win_panic("[TRY-CATCH] Fatal Error: Can't read exception error message!"); \
		catch_code \
		_DEBUG_FreeErrorCode(); \
		last_error = 0; \
	} \
	else {\
		__code__; \
	} \
	_DEBUG_ReleaseErrorCatchContext(__isErrorOccured); \
}