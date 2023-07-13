#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

#define INDEX_ADD_FUNCTION  0
#define INDEX_PRINT_FUNCTION  1

// обобщённая структура
typedef struct _OperationsType OperationsType;

// тип указателя на обобщённую функцию сложения
typedef OperationsType * (*OperationProto) (OperationsType *a, OperationsType *b);
// тип указателя на обобщённую функцию вывода
typedef void * (*OperationProtoPrint) (OperationsType *a);

struct _OperationsType {
  
    OperationProto *vtable; // указатель на массив указателей на функции
};



typedef struct _OperationsTypeInt {
    
    // содержит функциональность
    OperationsType head;

    // содержит данные
    int value;

} OperationsTypeInt;


typedef struct _OperationsTypeDouble {

    // содержит функциональность
    OperationsType head;

    // содержит данные
    double value;

} OperationsTypeDouble;

// полиморфная функция, умеющая складывать сущности различного типа
OperationsType* Add (OperationsType *a, OperationsType *b) {
    
    return a->vtable[INDEX_ADD_FUNCTION] (a, b);
}


OperationsTypeInt * AddInt (OperationsTypeInt *a, OperationsTypeInt *b) {

OperationsTypeInt *sum = (OperationsTypeInt*) malloc (sizeof(OperationsTypeInt));

    sum->head = a->head;
    sum->value = a->value + b->value;

    return sum;
}


OperationsTypeDouble * AddDouble (OperationsTypeDouble *a, OperationsTypeDouble *b) {

OperationsTypeDouble *sum = (OperationsTypeDouble*) malloc (sizeof(OperationsTypeDouble));

    sum->head = a->head;
    sum->value = a->value + b->value;

    return sum;
}


void PrintInt (OperationsTypeInt *a) {
    
    printf ("%d\n", a->value);
}

void PrintDouble (OperationsTypeDouble *a) {
    
    printf ("%f\n", a->value);
}

void Print (OperationsType *a) {
    
    ((OperationProtoPrint)a->vtable[INDEX_PRINT_FUNCTION]) (a);
}

// массив указателей на функции, работающих с OperationsTypeInt
OperationProto vtableInt[] = {(OperationProto)AddInt, (OperationProto) PrintInt};
// массив указателей на функции, работающих с OperationsTypeDouble
OperationProto vtableDouble[] = {(OperationProto)AddDouble, (OperationProto) PrintDouble};


int main () {



OperationsTypeInt intA = {vtableInt, 10};
OperationsTypeInt intB = {vtableInt, 20};
OperationsTypeInt *intSum;
OperationsTypeDouble doubleA = {vtableDouble, 1.0};
OperationsTypeDouble doubleB = {vtableDouble, 2.0};
OperationsTypeDouble *doubleSum;

    intSum = Add (&intA, &intB);
    doubleSum = Add (&doubleA, &doubleB);

    Print (intSum);
    Print (doubleSum);


    return 0;
}

/*
typedef struct _OperationsType OperationsType;

typedef OperationsType* (*OperationProto) (OperationsType *a, OperationsType *b);

// обобщённый тип (не используется для хранения данных)
struct _OperationsType {

    OperationProto *vtable;

};

typedef struct _OperationsTypeInt {

    //OperationProto *vtable;
    OperationsType head;
    int value;

} OperationsTypeInt;

typedef struct _OperationsTypeDouble {

    //OperationProto *vtable;
    OperationsType head;
    double value;

} OperationsTypeDouble;


// обобщённая функция
OperationsType* Add (OperationsType *a, OperationsType *b) {


    return a->vtable[0](a, b);
}


OperationsTypeInt * AddInt (OperationsTypeInt *a, OperationsTypeInt *b) {

OperationsTypeInt *sum = (OperationsTypeInt*) malloc (sizeof (OperationsTypeInt));

    sum->head = a->head;

    sum->value = a->value + b->value;

    return sum;
}


OperationsTypeDouble * AddDouble (OperationsTypeDouble *a, OperationsTypeDouble *b) {

OperationsTypeDouble *sum = (OperationsTypeDouble*) malloc (sizeof (OperationsTypeDouble));

    sum->head = a->head;

    sum->value = a->value + b->value;

    return sum;
}





int main () {

OperationProto vtableInt[1] = {(OperationProto)AddInt};
OperationProto vtableDouble[1] = {(OperationProto)AddDouble};

OperationsTypeInt intA = {vtableInt, 10};
OperationsTypeInt intB = {vtableInt, 20};
OperationsTypeInt *intSum;

OperationsTypeDouble doubleA = {vtableDouble, 2.0};
OperationsTypeDouble doubleB = {vtableDouble, 3.0};
OperationsTypeDouble *doubleSum;

    intA.value = 10;
    
    //intSum = (OperationsTypeInt *)Add ((OperationsType*)&intA, (OperationsType*)&intB);
    //doubleSum = (OperationsTypeDouble *)Add ((OperationsType*)&doubleA, (OperationsType*)&doubleB);
    intSum = (OperationsTypeInt *)Add (&intA.head, &intB.head);
    doubleSum = (OperationsTypeDouble *)Add (&doubleA.head, &doubleB.head);

    return 0;
}

*/