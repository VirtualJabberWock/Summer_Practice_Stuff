#pragma once
#include "Stream.h"
#include "..\Functional.h"
#include "..\CommonTypes.h"

STREAM_CLASS_FM(ByteOutputStream,

	size_t pointer;
,

	void (*writeUTF) (struct tagByteOutputStream* self, const char* str);
	void (*writeInt32) (struct tagByteOutputStream* self, __int32 value);
	void (*writeInt64) (struct tagByteOutputStream* self, __int64 value);
	void (*writeDouble) (struct tagByteOutputStream* self, double value);
	void (*writeFloat) (struct tagByteOutputStream* self, float value);
	void (*writeByte) (struct tagByteOutputStream* self, byte_t value);

);

ByteOutputStream* NewByteOutputStream();

STREAM_CLASS_FM(ByteInputStream,

	size_t pointer;
,

	String* (*readUTF) (struct tagByteInputStream* self);
	__int32 (*readInt32) (struct tagByteInputStream* self);
	__int64 (*readInt64) (struct tagByteInputStream* self);
	double (*readDouble) (struct tagByteInputStream* self);
	float (*readFloat) (struct tagByteInputStream* self);
	byte_t (*readByte) (struct tagByteInputStream* self);
);

typedef ByteInputStream ByteInputStreamRef;

ByteInputStream* NewByteInputStream(byte_t* array, int len);

/*Copy all data from some Stream*/
ByteInputStream* ByteIS_FromStream(StreamInterface* from);

/*Just cast stream to ByteInputStream,
when this->free() or this->clear() is calling, it is just do Nothing*/
ByteInputStreamRef* CastToByteInputStream(StreamInterface* from);
