#include <stdio.h>
#include "Person.h"
#include "..\core\Vector.h"
#include "..\core\Functional.h"
#include "..\core\DebugUtil.h"
#include "..\core\CommonTypes.h"
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

int main() {

	ObjectVector* v = NewObjectVector(2);
	v->_->push(v, NewString("Virtual Riot"));
	v->_->push(v, NewString("Infekt"));
	v->_->push(v, NewString("Eliminate"));
	v->_->push(v, NewString("John Wick"));
	printf("%s", CastString(v->_->get(v, 0))->ptr);
	v->free(v);
	free(v);
	return 0;

}