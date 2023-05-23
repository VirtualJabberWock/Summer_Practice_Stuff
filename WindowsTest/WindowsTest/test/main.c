#include <stdio.h>
#include "Person.h"
#include "..\core\Functional.h"


int main() {

	Person* p = NewPerson(19, "AquaHaze");
	if (p == 0) return -1;
	p->_mt->print = _DebugObjectMethod;
	p->_mt->print(p, 1, 2, 3);
	system("pause");
	return 0;

}