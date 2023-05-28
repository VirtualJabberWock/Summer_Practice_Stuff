#include <stdio.h>
#include "Person.h"
#include "..\core\Vector.h"
#include "..\core\Functional.h"
#include "..\core\DebugUtil.h"
#include "..\core\CommonTypes.h"
#include "..\core\List.h";
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

	ObjectList* list = NewObjectList();
	ObjectList* global = NewObjectList();

	list->_->push(list, NewString("1234567"));
	list->_->push(list, NewString("123456"));
	list->_->push(list, NewString("12345"));
	list->_->push(list, NewString("1234"));
	list->_->push(list, NewString("123"));
	list->_->push(list, NewString("12"));

	global->_->push(global, list);
	global->_->push(global, NewString("test"));

	/*for (ObjectNode* node = list->head; node != NULL; node = node->next) {
		printf("%s\n", CastString(node->obj)->ptr);
	}*/

	String* str = ListToString(list);
	printf("%s", str->ptr);
	DISPOSE_OBJECT(str);

	String* str0 = ListToString(global);
	printf("%s", str0->ptr);
	DISPOSE_OBJECT(str0);

	list->free(list);
	global->free(global);
	//free(list);
	free(global);

	return 0;

}