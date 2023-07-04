#pragma once

#include "..\CommonTypes.h"
#include "..\HashMap.h"
#include "..\Functional.h"
#include "..\io\ByteStream.h"


#define DESERIALIZATION_ERR_CODE 0xBadDe5e91al
#define SERIALIZATION_ERR_CODE 0xBad5e91al
#define BAD_DESERIALIZATION_EXCEPTION(str) {panic_custom(__func__, __LINE__, __FILE__, str, 0xBadDe5e91al); return 0;}
#define BAD_SERIALIZATION_EXCEPTION(str) {panic_custom(__func__, __LINE__, __FILE__, str, 0xBad5e91al); return 0;}

#define SERIAL_CLASS_FM(name, fields, methods) typedef struct name##_mtable_tag \
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
	ObjectType __objType; \
	void (*writeToStream) (struct tag##name* self, ByteOutputStream* s); \
	void (*readFromStream) (struct tag##name* self, ByteOutputStream* s); \
	fields \
\
} name; // END OF MACROS

__declspec(dllexport) char* Serializable_TYPE();

typedef struct tagSerializableObject {
	__int16 __header;
	ObjectType __type;
	void (*free) (Object* obj);
	int (*compare) (Object* self, Object* other, __int64* opt_OutHash);
	_methods_table* _;
	ObjectType __objType;
	void (*writeToStream) (Object* self, ByteOutputStream* s);
	void (*readFromStream) (Object* self, ByteOutputStream* s);
} SerializableObject;


#define SERIALCLASS_SUPER_FM(name, ptr) \
 ptr->__header = 1337; \
 ptr->__type = Serializable_TYPE; \
 ptr->free = standartFree;\
 ptr->compare = standartCompare; \
 ptr->_ = name##_METHODS; \
 ptr->__objType = name##_TYPE; \
 ptr->writeToStream = name##_writeToStream; \
 ptr->readFromStream = name##_readFromStream; \


void ValidateSerializationSignature(ByteInputStream* is, ObjectType type);
CanThrowAnException AddSerializationSignature(ByteOutputStream* os, ObjectType type);

/* @deprecated Not safe function!!! */
CanThrowAnException FastDeserialization(ByteInputStream* is, SerializableObject* ptr, size_t size_of_struct);
/* @deprecated Not safe function!!! */
CanThrowAnException FastSerialization(ByteOutputStream* os, SerializableObject* ptr, size_t size_of_struct);

