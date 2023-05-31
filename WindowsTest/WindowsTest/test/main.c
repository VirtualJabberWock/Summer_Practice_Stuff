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

int main() {

	HashMap* map = NewHashMap();
	map->_->emplace_by_str(map, "age", NewString("13"));
	map->_->emplace_by_str(map, "name", NewString("Bill"));
	map->_->emplace_by_str(map, "surname", NewString("Gates"));
	Person* p = PersonFromMap(map);
	p->_->print(p);
	map->free(map);
	free(map);
	p->free(p);
	free(p);
	return 0;

}