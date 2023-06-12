#include <stdio.h>
#include "Person.h"
#include "..\core\Vector.h"
#include "..\core\Functional.h"
#include "..\core\DebugUtil.h"
#include "..\core\CommonTypes.h"
#include "..\core\List.h";
#include "..\core\HashMap.h"
#include <stdlib.h>


void safeWrap(ObjectVector* v) {
	v->_->push(v, NewString("first"));
}

int OnErrorListener(const char* msg, unsigned int ErrorCode) {
	if (ErrorCode == ERR_INVALID_AGE) {
		printf("\nTrying to set invalid age to Person... Cancel operation...\n");
		return 1;
	}
	//printf("\n...\nUnhandled error: %s\n...\n",msg);
	//system("pause");
	return 0;
}

#include <string.h>
#include "..\core\io\Stream.h"

int main(int argc, char** argv) {

	IStream* stream = NewIStream();

	byte_t buffer[8] = { 0,1,2,3,4,5,6,7 };
	byte_t* b2 = calloc(4, sizeof(byte_t));
	stream->writeBytes(stream, buffer, 8);
	stream->readBytes(stream, &b2, 4, 0);
	for (int i = 0; i < 4; i++) {
		printf("%0.2hx ", b2[i]);
	}
	DISPOSE_OBJECT(stream);

	return 0;

}