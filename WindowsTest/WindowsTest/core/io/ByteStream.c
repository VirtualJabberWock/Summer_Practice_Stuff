#include "ByteStream.h"
#include <stdlib.h>
#include "..\DebugUtil.h"
#include <string.h>

DEFINE_TYPE(ByteOutputStream);
DEFINE_TYPE(ByteInputStream);

void ByteIS_writeUTF(ByteOutputStream* self, const char* str){
	int str_len = strlen(str);
	self->_->writeInt32(self, str_len);
	self->writeBytes(self, str, str_len);
	self->pointer += str_len;
}

void ByteIS_writeInt32(ByteOutputStream* self, __int32 value){
	NativeNumber32 n; n.n = value;
	self->writeBytes(self, n.bytes, sizeof(__int32));
	self->pointer += sizeof(__int32);
}

void ByteIS_writeInt64(ByteOutputStream* self, __int64 value){
	NativeNumber n; n.n = value;
	self->writeBytes(self, n.bytes, sizeof(__int64));
	self->pointer += sizeof(__int64);
}

void ByteIS_writeDouble(ByteOutputStream* self, double value){
	NativeNumber n; n.d = value;
	self->writeBytes(self, n.bytes, sizeof(double));
	self->pointer += sizeof(double);
}

void ByteIS_writeFloat(ByteOutputStream* self, float value){
	NativeNumber32 n; n.d = value;
	self->writeBytes(self, n.bytes, sizeof(float));
	self->pointer += sizeof(float);
}

void ByteIS_writeByte(ByteOutputStream* self, byte_t value){
	self->writeBytes(self, &value, 1);
	self->pointer += 1;
}

struct ByteOutputStream_mtable_tag ByteOutputStream_METHODS[] = {
	{
		ByteIS_writeUTF,
		ByteIS_writeInt32,
		ByteIS_writeInt64,
		ByteIS_writeDouble,
		ByteIS_writeFloat,
		ByteIS_writeByte
	}
};

ByteOutputStream* NewByteOutputStream()
{
	ByteOutputStream* is = calloc(1, sizeof(ByteOutputStream));
	if (is == 0) MEM_PANIC_EXCEPTION;
	STREAM_SUPER_FM(ByteOutputStream, is);
	is->bytes = calloc(is->capacity, sizeof(byte_t));
	return is;
}

String* ByteOS_readUTF(ByteInputStream* self) {
	int str_len = self->_->readInt32(self);
	if (str_len <= 0 || str_len > INT_MAX / 2) return TempString_("null", 4);
	char* buffer = calloc(str_len + 1, sizeof(char));
	if (buffer == 0) MEM_PANIC_EXCEPTION;
	self->readBytes(self, &buffer, str_len, self->pointer);
	self->pointer += str_len;
	buffer[str_len] = '\0';
	String* tmp = TempString_(buffer, str_len);
	free(buffer);
	return tmp;
}

__int32 ByteOS_readInt32(ByteInputStream* self) {
	if (sizeof(__int32) + self->pointer > self->size) return 0;
	NativeNumber32 n; memcpy_s(n.bytes, sizeof(__int32), self->bytes + self->pointer, sizeof(__int32));
	self->pointer += sizeof(__int32);
	return n.n;
}

__int64 ByteOS_readInt64(ByteInputStream* self) {
	if (self == 0) throw NULL_POINTER_EXCEPTION;
	if (sizeof(__int64) + self->pointer > self->size) return 0;
	NativeNumber n; memcpy_s(n.bytes, sizeof(__int64), self->bytes + self->pointer, sizeof(__int64));
	self->pointer += sizeof(__int64);
	return n.n;
}

double ByteOS_readDouble(ByteInputStream* self) {
	if (sizeof(double) + self->pointer > self->size) return 0;
	NativeNumber n; memcpy_s(n.bytes, sizeof(double), self->bytes + self->pointer, sizeof(double));
	self->pointer += sizeof(double);
	return n.d;
}

float ByteOS_readFloat(ByteInputStream* self) {
	if (sizeof(float) + self->pointer > self->size) return 0;
	NativeNumber32 n; memcpy_s(n.bytes, sizeof(float), self->bytes + self->pointer, sizeof(float));
	self->pointer += sizeof(float);
	return n.d;
}

byte_t ByteOS_readByte(ByteInputStream* self) {
	self->pointer += 1;
	return self->bytes[self->pointer-1];
}

struct ByteInputStream_mtable_tag ByteInputStream_METHODS[] = {
	{
		ByteOS_readUTF,
		ByteOS_readInt32,
		ByteOS_readInt64,
		ByteOS_readDouble,
		ByteOS_readFloat,
		ByteOS_readByte
	}
};

ByteInputStream* NewByteInputStream(byte_t* array, int len)
{
	ByteInputStream* is = calloc(1, sizeof(ByteInputStream));
	if (is == 0) MEM_PANIC_EXCEPTION;
	is->bytes = 0;
	STREAM_SUPER_FM(ByteInputStream, is);
	is->size = len;
	is->capacity = len;
	if (is->bytes != 0) free(is->bytes);
	is->bytes = calloc(len, sizeof(byte_t));
	if(is->bytes == 0) MEM_PANIC_EXCEPTION;
	for (int i = 0; i < len; i++) {
		is->bytes[i] = array[i];
	}
	return is;
}


ByteInputStream* ByteIS_FromStream(StreamInterface* from)
{
	ByteInputStream* is = calloc(1, sizeof(ByteInputStream));
	if (is == 0) MEM_PANIC_EXCEPTION;
	is->bytes = 0;
	STREAM_SUPER_FM(ByteInputStream, is);
	is->size = from->size;
	is->capacity = from->capacity;
	if (is->bytes != 0) free(is->bytes);
	is->bytes = calloc(from->size, sizeof(byte_t));
	if (is->bytes == 0) MEM_PANIC_EXCEPTION;
	for (int i = 0; i < from->size; i++) {
		is->bytes[i] = from->bytes[i];
	}
	return is;
}

/**/
ByteInputStreamRef* CastToByteInputStream(StreamInterface* from)
{
	ByteInputStream* is = calloc(1, sizeof(ByteInputStream));
	if (is == 0) MEM_PANIC_EXCEPTION;
	is->bytes = 0;
	STREAM_SUPER_FM(ByteInputStream, is);
	is->size = from->size;
	is->capacity = from->capacity;
	is->bytes = from->bytes;
	is->free = DoNothing;
	is->clear = DoNothing;
	return is;
}


