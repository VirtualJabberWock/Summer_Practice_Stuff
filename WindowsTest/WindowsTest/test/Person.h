#pragma once
#include "..\core\Object.h"
#include "..\core\HashMap.h"

#define ERR_INVALID_AGE 999999

OBJECT_CLASS_FM(Person,

	int age;
	const char* name;
	const char* surname;
,   
	void (*print)(struct tagPerson* p);
	void (*setAge)(struct tagPerson* p, int age);

);

Person* NewPerson(int age, const char* name);
Person* PersonFromMap(HashMap* dict);