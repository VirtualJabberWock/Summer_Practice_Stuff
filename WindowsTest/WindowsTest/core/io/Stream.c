#include "Stream.h"
#include <stdlib.h>
#include "..\DebugUtil.h"

char* Stream_TYPE()
{
	return "Stream";
}

int checkStreamType(StreamInterface* s, ObjectType type)
{
	if(checkObjectType(s, Stream_TYPE) == 0) return 0;
	return (s->__typeStream == type);
}

void IStream_Free(StreamInterface* s)
{
	if (s == 0) return;
	if(checkObjectType(s, Stream_TYPE) == 0) return;
	if (s->bytes != 0) free(s->bytes);
	s->bytes = 0;
	s->size = 0;
}

void IStream_clear(StreamInterface* s)
{
	if (s == 0) return;
	if (checkObjectType(s, Stream_TYPE) == 0) return;
	if (s->bytes != 0) free(s->bytes);
	s->capacity = STREAM_DEFAULT_CAPACITY;
	s->bytes = calloc(STREAM_DEFAULT_CAPACITY, sizeof(byte_t));
	s->size = 0;
}

void IStream_writeBytes(StreamInterface* s, byte_t* bytes, int len)
{
	if (s == 0) throw NULL_POINTER_EXCEPTION;
	if (checkObjectType(s, Stream_TYPE) == 0) return;
	int t = len + s->size;
	if (t > s->capacity) {
		if (len > 4) s->capacity += len * 1.5;
		else s->capacity += 8 + len;
		byte_t* ptr_ = (byte_t*)realloc(s->bytes, sizeof(byte_t) * s->capacity);
		if (ptr_ == NULL) throw MEM_PANIC_EXCEPTION;
		s->bytes = ptr_;
	}
	for (int i = 0; i < len; i++) {
		s->bytes[s->size] = bytes[i];
		s->size++;
	}
}

void IStream_readBytes(StreamInterface* s, byte_t** out, int bytesToRead, int offset)
{
	int toRead = bytesToRead;
	if (s->size <= bytesToRead) {
		toRead = s->size;
	}
	if (s == 0) throw NULL_POINTER_EXCEPTION;
	if (out == 0) throw NULL_POINTER_EXCEPTION;
	if (*out == 0 || *out == UNEXPECTED_PTR0 || *out == UNEXPECTED_PTR2) throw NULL_POINTER_EXCEPTION;
	if (checkObjectType(s, Stream_TYPE) == 0) return;
	if (offset >= 0) {
		int j = 0;
		for (int i = offset; i < toRead+offset; i++) {
			if (i >= s->size) break; //TODO: think about it
			byte_t* to = *out;
			to[j] = s->bytes[i];
			j++;
		}
	}
	else {
		if (s->size + offset < 0) return;
		int j = 0;
		for (int i = s->size+offset; i < s->size; i++) {
			byte_t* to = *out;
			to[j] = s->bytes[i];
			j++;
		}
	}
}

size_t IStream_readAllBytes(StreamInterface* s, byte_t** out)
{
	if (s == 0) throw NULL_POINTER_EXCEPTION;
	if (out == 0) throw NULL_POINTER_EXCEPTION;
	if (*out == 0) *out = calloc(s->size, sizeof(byte_t));
	IStream_readBytes(s, out, s->size, 0);
	return s->size;
}

void IStream_add_zero_bytes(StreamInterface* s, size_t count)
{
	int t = count + s->size;
	if (t > s->capacity) {
		s->capacity += count;
		byte_t* ptr_ = (byte_t*)realloc(s->bytes, sizeof(byte_t) * s->capacity);
		if (ptr_ == NULL) throw MEM_PANIC_EXCEPTION;
		s->bytes = ptr_;
	}
	s->size += count;
}

StreamInterface* NewStream()
{
	StreamInterface* ptr = (StreamInterface*)calloc(1, sizeof(StreamInterface));
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
	ptr->bytes = calloc(ptr->capacity, sizeof(byte_t));
	return ptr;
}

#include <string.h>

StreamInterface* NewStreamFromNTString(const char* nt_c_str)
{
	StreamInterface* s = NewStream();
	s->capacity = 5;
	s->size = 0;
	int i = 0;
	while (nt_c_str[i] != '\0') {
		s->bytes[s->size] = nt_c_str[i];
		if (i > s->capacity) {
			s->capacity = s->capacity * 2;
			byte_t* ptr_ = (byte_t*)realloc(s->bytes, sizeof(byte_t) * s->capacity);
			if (ptr_ == NULL) throw MEM_PANIC_EXCEPTION;
			ptr_[s->size - 1] = s->bytes[s->size - 1];
			s->bytes = ptr_;
		}
		s->bytes[s->size] = nt_c_str[i];
		s->size++;
		i++;
	}
	return s;
}
