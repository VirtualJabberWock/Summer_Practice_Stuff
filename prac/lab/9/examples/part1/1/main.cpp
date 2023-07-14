#include <stdio.h>

#undef _STD_USING
#include <cstdio>
//#include <iostream>


namespace NS1 {

void OpenFile () {

    printf ("OpenFile in NS1\n");
    return;
}

namespace NS1 {

void OpenFile () {

	printf ("OpenFile in NS1::NS1\n");
    return;
}
    
}
    
}


namespace NS2 {

void OpenFile () {

    printf ("OpenFile in NS2\n");
    return;
}
    
}

// подставляет все имена из пространства имён NS1 в текущее
//using namespace NS1;

// подставляет одно имя из пространства имён
using NS2::OpenFile;


/*
void OpenFile () {

    printf ("OpenFile\n");
    return;
}
//*/



#define SOME_CONST  10
const int glSomeConst = 10;
char buf[glSomeConst];


/*
int a;
int b = a;

int &rA = a;

    rA = a;

    int buf[] = {1, 2};
    buf[] = {1, 2};

int var = 10;   // инициализация
    var = 10;   // присваивание

    rA = 10;
    rA = b;

    b = a;
    a = b;
//*/




void fun2 (int &arg) {

    arg = 10;
    
    return;
}


void fun (int &arg) {


    arg = 10;
    
    return;
}


#define IN
#define OUT
// рекомендуется выходные значения по прежнему передавать по указателю
void fun (IN const int &argIn, IN OUT int * const argOut) {

    //argIn = ...
    //argIn = 10;
    *argOut = argIn;
}


/*
int fun (int &argIn, int *argOut) {

    //argIn = ...
    *argOut = argIn;

    return 1;
}
*/



// В языке С++ пустой список аргументов означает отсутствие аргументов
// в отличие от языка С, в котором это означало произвольное количество аргументов
void EmptyArg () {
    
}


void TestFun (int &a) {

    a = 10;
}


void ShowConst () {

    // изменять константу нельзя
    //glSomeConst = 1;
    /*
    int *p = &glSomeConst;
    *p = 5;
    //*/
    int someConst = glSomeConst;
    //const int *p = &glSomeConst;
    //const int localConst;
    


    int a = 0;
    // обычная ссылка
    int &rA = a;
    // константная ссылка
    // преобразование из &int в const &int (разрешено, так как безопасно)
    const int &cRA = rA;
	//int &rCRA = cRA;
    a = 10;
    //cRA = 10;

    rA = 10;
    //cRA = 11;

    int b = 0;
    // ошибка преобразования из const &int в &int (запрещено, т.к. потенциально небезопасно)
    const int &cRB = b;
    int &rB = b;
    //int &rB = cRB;

    // ошибка преобразования const int * в int *
    int bb = *&cRB;
    //*&cRB = 10;
    //int * pB = &cRB;
    const int * pB1 = &cRB;
    int const * pB2 = &cRB;
    const int * pB3 = &b;
    // так нельзя
    //*pB1 = 1;   // const &int = 1;

    int c;

    // обычный указатель на a
    int *pA = &a;
    // константый указатель на a (указатель нельзя менять)
    pA = &b;
    int * const pCA = &a;
    // нельзя переприсвоить значение cPA
    //pCA = &b;
    //pCA++;
    *pCA = 1;
    
    // указатель на константу
    const int *cPA = &cRA;
    //int const * cPA = &cRA;   // то же, что и выше
    // нельзя
    //*cPA = c;
    c = *cPA;

    // константный указатель на константу
    const int * const cPCA = &a;
    //int const * const cPCA = &a;

    return;
}


int a = 100;

int main () {

    int a = 0;
    a += 1;

    NS1::OpenFile();
	NS1::NS1::OpenFile();
    NS2::OpenFile();
    OpenFile();
    //::OpenFile();

    ShowConst();

    //int &rA2 = 10;
    //fun2 (10);
    fun2 (a);
    printf ("%d\n", a);
    printf ("%d\n", ::a);
    // объявление не в начале блока
    int b = 0;

    fun (a);
    fun (a, &a);
    printf ("b = %d\n", b);

    EmptyArg ();
    //EmptyArg (0, 1, 2);


    // определение ссылки
    int &rA = a;
    // rA ссылается на то же место в памяти, что и a (можно сказать, что rA - синоним переменной a)
    // rA всегда будет ссылаться на a
    rA = b; // -> a = b
    // ошибка (переопределение переменной)
    //int &rA = b;

    std::printf ("Hello, World!\n");

    return 0;
}
