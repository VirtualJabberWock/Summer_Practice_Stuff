#pragma once
#include "Object.h"
#include "Functional.h"

void _DebugObjectMethod(Object* obj, ...);

static int exit_if_error = 1;
static char* last_error = 0;
static unsigned int last_error_code = 0;
static int last_check = 0;

static const unsigned int ERR_CANT_ALLOC_MEM = 0xdeadbeef;
static const unsigned int ERR_NULL_POINTER = 0xCCCCCCCC;
static const unsigned int ERR_INVALID_CAST = 0xBADCA57;

int panic(const char* err_msg, unsigned int err_code);
int panic_npe(const char* func_name, int line, const char* filename);
int panic_cast(const char* to_type, const char* func_name, int line, const char* filename);

#define MEM_PANIC_RETURN_0 {panic("Can't alloc memory!", ERR_CANT_ALLOC_MEM); return 0;}
#define MEM_PANIC_RETURN_V {panic("Can't alloc memory!", ERR_CANT_ALLOC_MEM); return;}
#define NULL_POINTER_EXCEPTION {panic_npe(__func__, __LINE__, __FILE__); return;}
#define INVALID_CAST_EXCEPTION(name) {panic_cast(name,__func__, __LINE__, __FILE__); return;}
#define throw

int win_panic(const char* err_msg);
int unix_panic(const char* err_msg);

typedef int (*OnException) (const char* err, unsigned int err_code); //return 1 if exception was handled

/* return 1 if error, OnException can be NULL */
int TryToExecute(OnException optional, VoidWrap action, void* context);

/*
don't forget to free error msg ! and make it NULL again!
return 0 if okay, error message if error!
@deprecated
*/
const char* TryToExecuteSaveError(VoidWrap action, void* context);