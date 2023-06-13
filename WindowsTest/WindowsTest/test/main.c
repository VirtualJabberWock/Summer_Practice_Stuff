#include <stdio.h>
#include "Person.h"
#include "..\core\Vector.h"
#include "..\core\Functional.h"
#include "..\core\DebugUtil.h"
#include "..\core\CommonTypes.h"
#include "..\core\List.h";
#include "..\core\HashMap.h"
#include <stdlib.h>


//void safeWrap(ObjectVector* v) {
//	v->_->push(v, NewString("first"));
//}
//
//int OnErrorListener(const char* msg, unsigned int ErrorCode) {
//	if (ErrorCode == ERR_INVALID_AGE) {
//		printf("\nTrying to set invalid age to Person... Cancel operation...\n");
//		return 1;
//	}
//	printf("\n...\nUnhandled error: %s\n...\n",msg);
//	system("pause");
//	return 0;
//}

#include <string.h>
#include "..\core\io\Stream.h"
#include "..\core\io\CommonStreams.h"

int main(int argc, char** argv) {

	//Test 1 (Default Stream):

	StreamInterface* is = NewStream();

	byte_t buffer[8] = { 1,2,3,4,5,6,7,8 };
	byte_t* received = (byte_t*)calloc(4, sizeof(byte_t));

	is->writeBytes(is, buffer, 8);
	is->readBytes(is, &received, 4, False); // fromStart = False, it means it read last 'n' bytes from stream buffer;

	for(int i = 0; i < 4; i++) printf("%0.2hhx ", received[i]);
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