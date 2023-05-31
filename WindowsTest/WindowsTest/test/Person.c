#include "Person.h"
#include <stdlib.h>
#include <stdio.h>
#include "..\core\DebugUtil.h"

DEFINE_TYPE(Person);

void print(Person* p) {
	printf("Person: {\n\tname = %s;\n\tsurname = %s;\n\tage = %d;\n}\n", p->name,p->surname, p->age);
}

void setAge(Person* p, int age) {
	if (age > 0 && age < 999999) p->age = age;
	else panic("Age cannot be less than 0, or greater than 999999", ERR_INVALID_AGE);
}

struct Person_mtable_tag Person_METHODS[] = { { print, setAge } };

Person* NewPerson(int age, const char* name)
{
	Person* p = malloc(sizeof(Person));
	if (p == 0) MEM_PANIC_RETURN_0;
	OBJECT_SUPER_FM(Person, p);
	p->age = age;
	p->name = name;
	p->surname = "";
	return p;
}

#include "..\core\reflect\Serializable.h"

Person* PersonFromMap(HashMap* map)
{
	Person* p = NewPerson(0, "");
	p->age = atoi(getString(map, "age"));
	p->name = getString(map, "name");
	p->surname = getString(map, "surname");
	return p;
}
