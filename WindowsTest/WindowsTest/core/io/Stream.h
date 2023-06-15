#pragma once
#include "..\Object.h"
#include "..\CommonTypes.h"

#define STREAM_DEFAULT_CAPACITY 5


#define STREAM_CLASS_FM(name, fields, methods) typedef struct name##_mtable_tag \
{ \
methods \
} name##_mtable; \
__declspec(dllexport) char* name##_TYPE(); \
\
typedef struct tag##name { \
\
	__int16 __header; \
	__int64 __type; \
	void (*free) (struct tag##name* obj); \
    int (*compare) (struct tag##name* self, struct tag##name* other, __int64* opt_OutHash);\
	name##_mtable* _; \
	ObjectType __typeStream; \
	void (*clear) (struct tag##name* stream); \
	void (*writeBytes) (struct tag##name* s, byte_t* arr, int len); \
	void (*readBytes) (struct tag##name* s, byte_t** out, int bytesToRead, bool_t fromStart); \
	void (*readAllBytes) (struct tag##name* s, byte_t** out, int* outTotalLength); \
	__int64 size; \
	__int64 capacity;\
	byte_t* bytes; \
	fields \
\
} name; // END OF MACROS

__declspec(dllexport) char* Stream_TYPE();

typedef struct tagIStream {
	__int16 __header; 
	ObjectType __type; 
	void (*free) (struct tagIStream* obj);
	int (*compare) (struct tagIStream* self, struct tagIStream* other, __int64* opt_OutHash);
	_methods_table* _;
	ObjectType __typeStream;
	void (*clear) (struct tagIStream* stream);
	void (*writeBytes) (struct tagIStream* s, byte_t* arr, int len);
	void (*readBytes) (struct tagIStream* s, byte_t** out, int bytesToRead, bool_t fromStart);
	int (*readAllBytes) (struct tagIStream* s, byte_t** out);
	__int64 size; 
	__int64 capacity;
	byte_t* bytes;
} StreamInterface;


#define STREAM_SUPER_FM(name, ptr) \
 ptr->__header = 1337; \
 ptr->__type = Stream_TYPE; \
 ptr->free = IStream_Free;\
 ptr->compare = standartCompare; \
 ptr->_ = name##_METHODS; \
 ptr->__typeStream = name##_TYPE; \
 ptr->size = 0; \
 ptr->clear = IStream_clear; \
 ptr->writeBytes = IStream_writeBytes; \
 ptr->readBytes = IStream_readBytes; \
 ptr->readAllBytes = IStream_readAllBytes; \
 ptr->capacity = 5; 

int checkStreamType(StreamInterface* s, ObjectType type);

void IStream_Free(StreamInterface* s);
void IStream_clear(StreamInterface* s);
void IStream_writeBytes(StreamInterface* s, byte_t* bytes, int len);
void IStream_readBytes(StreamInterface* s, byte_t** out, int bytesToRead, bool_t fromStart);
int IStream_readAllBytes(StreamInterface* s, byte_t** out);

StreamInterface* NewStream();
StreamInterface* NewStreamFromNTString(const char* nt_c_str);