#include "Person.h"
#include <stdlib.h>
#include <stdio.h>

void print(Person* p) {
	printf("Person: {\n\tname = %s;\n\tage = %d;\n}", p->name, p->age);
}

struct Person_mtable_tag Person_METHODS[] = { { print } };

Person* NewPerson(int age, const char* name)
{
	Person* p = malloc(sizeof(Person));
	if (p == 0) return 0;
	OBJECT_SUPER_FM(Person, p);
	p->age = age;
	p->name = name;
	return p;
}
