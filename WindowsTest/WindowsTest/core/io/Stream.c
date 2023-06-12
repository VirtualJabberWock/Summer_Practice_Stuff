#include "Stream.h"
#include <stdlib.h>
#include "..\DebugUtil.h"

char* Stream_TYPE()
{
	return "Stream";
}

int checkStreamType(IStream* s, ObjectType type)
{
	if(checkObjectType(s, Stream_TYPE) == 0) return 0;
	return (s->__typeStream == type);
}

void IStream_Free(IStream* s)
{
	if (s == 0) return;
	if(checkObjectType(s, Stream_TYPE) == 0) return;
	if (s->bytes != 0) free(s->bytes);
	s->bytes = 0;
	s->size = 0;
}

void IStream_clear(IStream* s)
{
	if (s == 0) return;
	if (checkObjectType(s, Stream_TYPE) == 0) return;
	if (s->bytes != 0) free(s->bytes);
	s->capacity = STREAM_DEFAULT_CAPACITY;
	s->bytes = calloc(STREAM_DEFAULT_CAPACITY, sizeof(byte_t));
	s->size = 0;
}

void IStream_writeBytes(IStream* s, byte_t* bytes, int len)
{
	if (s == 0) throw NULL_POINTER_EXCEPTION;
	if (checkObjectType(s, Stream_TYPE) == 0) return;
	int t = len + s->size;
	if (t > s->capacity) {
		s->capacity += (int)(len * 1.25);
		byte_t* ptr_ = (byte_t*)realloc(s->bytes, sizeof(byte_t) * s->capacity);
		if (ptr_ == NULL) throw MEM_PANIC_RETURN_V;
		s->bytes = ptr_;
	}
	for (int i = 0; i < len; i++) {
		s->bytes[s->size] = bytes[i];
		s->size++;
	}
}

void IStream_readBytes(IStream* s, byte_t** out, int bytesToRead, bool_t fromStart)
{
	int toRead = bytesToRead;
	if (s->size <= bytesToRead) {
		toRead = s->size;
	}
	if (s == 0) throw NULL_POINTER_EXCEPTION;
	if (out == 0) throw NULL_POINTER_EXCEPTION;
	if (checkObjectType(s, Stream_TYPE) == 0) return;
	if (fromStart) {
		for (int i = 0; i < toRead; i++) {
			byte_t* to = *out;
			to[i] = s->bytes[i];
		}
	}
	else {
		int start_pos = s->size - bytesToRead;
		int j = 0;
		for (int i = start_pos; i < s->size; i++) {
			byte_t* to = *out;
			to[j] = s->bytes[i];
			j++;
		}
	}
}

int IStream_readAllBytes(IStream* s, byte_t** out)
{
	if (s == 0) throw NULL_POINTER_EXCEPTION;
	if (out == 0) throw NULL_POINTER_EXCEPTION;
	if (*out == 0) throw NULL_POINTER_EXCEPTION;
	IStream_readBytes(s, out, s->size, 1);
	return s->bytes;
}

IStream* NewIStream()
{
	IStream* ptr = (IStream*)calloc(1, sizeof(IStream));
	if (ptr == 0) throw NULL_POINTER_EXCEPTION;
	ptr->__header = 1337; 
	ptr->__type = Stream_TYPE; 
	ptr->free = IStream_Free;
	ptr->compare = standartCompare; 
	ptr->__typeStream = Stream_TYPE;
	ptr->size = 0; 
	ptr->clear = IStream_clear; 
	ptr->writeBytes = IStream_writeBytes; 
	ptr->readBytes = IStream_readBytes; 
	ptr->readAllBytes = IStream_readAllBytes; 
	ptr->capacity = 5;
	return ptr;
}
