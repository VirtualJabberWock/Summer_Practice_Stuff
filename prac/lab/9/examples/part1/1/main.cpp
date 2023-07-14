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

// ����������� ��� ����� �� ������������ ��� NS1 � �������
//using namespace NS1;

// ����������� ���� ��� �� ������������ ���
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

int var = 10;   // �������������
    var = 10;   // ������������

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
// ������������� �������� �������� �� �������� ���������� �� ���������
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



// � ����� �++ ������ ������ ���������� �������� ���������� ����������
// � ������� �� ����� �, � ������� ��� �������� ������������ ���������� ����������
void EmptyArg () {
    
}


void TestFun (int &a) {

    a = 10;
}


void ShowConst () {

    // �������� ��������� ������
    //glSomeConst = 1;
    /*
    int *p = &glSomeConst;
    *p = 5;
    //*/
    int someConst = glSomeConst;
    //const int *p = &glSomeConst;
    //const int localConst;
    


    int a = 0;
    // ������� ������
    int &rA = a;
    // ����������� ������
    // �������������� �� &int � const &int (���������, ��� ��� ���������)
    const int &cRA = rA;
	//int &rCRA = cRA;
    a = 10;
    //cRA = 10;

    rA = 10;
    //cRA = 11;

    int b = 0;
    // ������ �������������� �� const &int � &int (���������, �.�. ������������ �����������)
    const int &cRB = b;
    int &rB = b;
    //int &rB = cRB;

    // ������ �������������� const int * � int *
    int bb = *&cRB;
    //*&cRB = 10;
    //int * pB = &cRB;
    const int * pB1 = &cRB;
    int const * pB2 = &cRB;
    const int * pB3 = &b;
    // ��� ������
    //*pB1 = 1;   // const &int = 1;

    int c;

    // ������� ��������� �� a
    int *pA = &a;
    // ���������� ��������� �� a (��������� ������ ������)
    pA = &b;
    int * const pCA = &a;
    // ������ ������������� �������� cPA
    //pCA = &b;
    //pCA++;
    *pCA = 1;
    
    // ��������� �� ���������
    const int *cPA = &cRA;
    //int const * cPA = &cRA;   // �� ��, ��� � ����
    // ������
    //*cPA = c;
    c = *cPA;

    // ����������� ��������� �� ���������
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
    // ���������� �� � ������ �����
    int b = 0;

    fun (a);
    fun (a, &a);
    printf ("b = %d\n", b);

    EmptyArg ();
    //EmptyArg (0, 1, 2);


    // ����������� ������
    int &rA = a;
    // rA ��������� �� �� �� ����� � ������, ��� � a (����� �������, ��� rA - ������� ���������� a)
    // rA ������ ����� ��������� �� a
    rA = b; // -> a = b
    // ������ (��������������� ����������)
    //int &rA = b;

    std::printf ("Hello, World!\n");

    return 0;
}
