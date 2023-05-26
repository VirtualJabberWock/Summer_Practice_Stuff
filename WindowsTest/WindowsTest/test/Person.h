#pragma once
#include "..\core\Object.h"

#define ERR_INVALID_AGE 999999

OBJECT_CLASS_FM(Person,

	int age;
	const char* name;
,   
	void (*print)(struct tagPerson* p);
	void (*setAge)(struct tagPerson* p, int age);

);

Person* NewPerson(int age, const char* name);