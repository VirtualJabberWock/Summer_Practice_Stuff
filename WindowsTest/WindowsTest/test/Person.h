#pragma once
#include "..\core\Object.h"

OBJECT_CLASS_FM(Person,

	int age;
	const char* name;
,   
	void (*print)(struct tagPerson* p);

);

Person* NewPerson(int age, const char* name);