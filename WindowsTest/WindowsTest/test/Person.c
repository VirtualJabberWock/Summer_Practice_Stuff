#include "Person.h"
#include <stdlib.h>
#include <stdio.h>
#include "..\core\DebugUtil.h"
#include "..\core\reflect\NativeBridge.h"

IMPORT_NATIVE_REFLECTION;

DEFINE_TYPE(Person);

void Person_print() {
	GET_CALLER_OBJECT(Person, some64magic0);
	printf("Person: {\n\tname = %s;\n\tsurname = %s;\n\tage = %d;\n}\n", __self__->name, __self__->surname, __self__->age);
}

void Person_setAge(Person* p, int age) {
	if (age > 0 && age < 999999) p->age = age;
	else panic("Age cannot be less than 0, or greater than 999999", ERR_INVALID_AGE);
}

void Person_setSurname(const char* surname) {
	GET_CALLER_OBJECT(Person, some64magic0);
	__self__->surname = copy_c_string(surname, 1000);
}

struct Person_mtable_tag Person_METHODS[] = { { Person_print, Person_setAge, Person_setSurname } };

Person* NewPerson(int age, const char* name)
{
	Person* p = malloc(sizeof(Person));
	if (p == 0) MEM_PANIC_EXCEPTION;
	OBJECT_SUPER_FM(Person, p);
	p->age = age;
	p->name = name;
	p->surname = "";
	return p;
}