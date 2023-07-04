#include <stdio.h>
#include "Person.h"
#include "..\core\Vector.h"
#include "..\core\Functional.h"
#include "..\core\DebugUtil.h"
#include "..\core\CommonTypes.h"
#include "..\core\List.h";
#include "..\core\HashMap.h"
#include <stdlib.h>
#include <string.h>
#include "..\core\io\Stream.h"
#include "..\core\io\ByteStream.h"
#include "..\core\io\CommonStreams.h"
#include "..\core\reflect\NativeBridge.h"

int StreamTest() {

	StreamInterface* is = NewStream();

	byte_t buffer[8] = { 1,2,3,4,5,6,7,8 };
	byte_t* received = (byte_t*)calloc(4, sizeof(byte_t));

	is->writeBytes(is, buffer, 8);
	is->readBytes(is, &received, 4, 4);

	for (int i = 0; i < 4; i++) printf("%0.2hhx ", received[i]);
	printf("\n\n");

	DISPOSE_OBJECT(is);
	free(received);

	//Test 2 (Stream from STDOUT):     

	StreamStdOut* out_stream = NewSTDOutStream(); // can be casted to StreamInterface (inheritor of StreamInterface)

	int a = 1337;

	out_stream->_
		->str("Hello, World!\n")
		->str("I can even put a number [")->num(a)->str("]!\n")
		->str("Test\n")
		->str("End of stream!\n");

	DISPOSE_OBJECT(out_stream);

	return 0;
}

#include "BigInteger.h"

IMPORT_TRY_CATCH;

int main(int argc, char** argv) {

	ByteOutputStream* os = NewByteOutputStream();
	
	os->_->writeInt64(os, 32);
	os->_->writeInt32(os, 1337);
	os->_->writeUTF(os, "Service");
	ByteInputStream* is = CastToByteInputStream(os);
	for (int i = 0; i < is->size; i++) {
		printf("%0.2hhx ", is->bytes[i]);
	}
	__int64 a = 0;
	TRY_TO_EXECUTE({
		__int64 a = is->_->readInt64(NULL);
	}, 
	CATCH{
		printf("\n\nFailed to readInt64(...), because of {\n\n%s\n\n}\n", DEBUG_GetLastError());
		is->pointer += 8;
	});
	int b = is->_->readInt32(is);
	String* str = is->_->readUTF(is);
	printf("\n%lld, %d, %s", a, b, str->ptr);
	DISPOSE_OBJECT(str);
	DISPOSE_OBJECT(os);
	DISPOSE_OBJECT(is);
	return 0;
}

//void safePersonChange(AnonymousContext* context) {
//	NativeAnonymousContext* n_context = Functional_getNativeContext(context);
//	if (n_context == NULL) throw NULL_POINTER_EXCEPTION;
//	Person* p = n_context->_[0].ptr;
//	int age = n_context->_[1].number;
//	printf("\n%llf\n", age);
//	if (!checkObjectType(p, Person_TYPE)) return;
//	p->_->setAge(p, age);
//}
//
//void MyExecptionHandler(const char* err, int err_code, SingleAnonymousContext* context) {
//	if (err_code == ERR_INVALID_AGE) {
//		printf("[ErrorLog]: %s\n", err);
//	}
//	return 0;
//}