#include "Serializable.h"
#include <stdlib.h>
#include <corecrt_memcpy_s.h>

#define __SERIALIZATION_SIGN 0x3390'9597'3386'3974 //first 16 bytes of md5 hash of string: "Serializable"

CanThrowAnException ValidateSerializationSignature(ByteInputStream* is, ObjectType type)
{
	if (is == 0) throw NULL_POINTER_EXCEPTION;
	is->pointer = 0;
	if (is->size == 0) 
		throw BAD_DESERIALIZATION_EXCEPTION("Bad deserialization (empty Stream)!");
	__int64 test = is->_->readInt64(is);
	if (test != __SERIALIZATION_SIGN)
		throw BAD_DESERIALIZATION_EXCEPTION("Invalid serialization signature!");
	__int64 testType = is->_->readInt64(is);
	if (testType != (__int64)type) 
		throw BAD_DESERIALIZATION_EXCEPTION("Wrong Object type!");
}

void AddSerializationSignature(ByteOutputStream* os, ObjectType type)
{
	if (os == 0) throw NULL_POINTER_EXCEPTION;
	os->pointer = 0;
	os->_->writeInt64(os, __SERIALIZATION_SIGN);
	os->_->writeInt64(os, (__int64)type);
}

CanThrowAnException FastDeserialization(ByteInputStream* is, SerializableObject* ptr, size_t full_size)
{
	if (checkObjectType(ptr, Serializable_TYPE) == 0)
		throw BAD_SERIALIZATION_EXCEPTION("[FastSerialization] Object not serializable!");

	if (is == 0)
		throw NULL_POINTER_EXCEPTION;

	if (full_size > 536870912)
		throw BAD_DESERIALIZATION_EXCEPTION("Very big object (maybe wrong size)!");

	size_t _offset = sizeof(SerializableObject);
	size_t size_of_struct = full_size - _offset;
	
    __int64 test = is->_->readInt64(is);
	if (test != size_of_struct) 
		throw BAD_DESERIALIZATION_EXCEPTION("[FastDeserialization] Wrong size, validation failed!");
	int is_ptr = is->pointer;
	memcpy_s(ptr + _offset, size_of_struct, (byte_t*)(is->bytes + is_ptr), size_of_struct);
}

CanThrowAnException FastSerialization(ByteOutputStream* os, SerializableObject* ptr, size_t full_size)
{

	if(checkObjectType(ptr, Serializable_TYPE) == 0) 
		throw BAD_SERIALIZATION_EXCEPTION("[FastSerialization] Object not serializable!");

	if (os == 0)
		throw NULL_POINTER_EXCEPTION;

	if (full_size > 536870912)
		throw BAD_SERIALIZATION_EXCEPTION("Very big object (maybe wrong size)!");

	size_t _offset = sizeof(SerializableObject);
	size_t size_of_struct = full_size - _offset;
	os->_->writeInt64(os, (__int64)size_of_struct);
	int current_size = os->size;
	IStream_add_zero_bytes(os, size_of_struct);
	memcpy_s((byte_t*)(os->bytes + current_size), size_of_struct, ptr+_offset, size_of_struct);
}

char* Serializable_TYPE()
{
	return "Serializable";
}
