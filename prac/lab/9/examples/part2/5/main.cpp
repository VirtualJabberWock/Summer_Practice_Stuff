
#include <iostream>

using std::cout;

class ParentClass {

public:

    void ParentMethod() {
        cout << "ParentMethod\n";
        }

    void SomeMethod() {
        cout << "Parent SomeMethod\n";
        }

    virtual void SomeVirtualFunction () {
        cout << "Virtual Parent\n";
        }

private:

};


// �������� ������������
class ChildClass: public ParentClass {
    
public:

    // ���������� ������� ������������� ������
    void SomeMethod() {
        cout << "Child SomeMethod\n";
        }

    virtual void SomeVirtualFunction () {
        cout << "Virtual Child\n";
        }

private:

    int value;

};


class ChildClass2: public ParentClass {

public:

    virtual void SomeVirtualFunction () {
        cout << "Virtual Child2\n";
        }

};


void SomeFunction (ParentClass *pc) {

    // pc - ����� ����������� ��� ParentClass

    //
    pc->ParentMethod();
    pc->SomeMethod();

    // ������������ (�������) ����������, �.�. �� ����� ����������
    // ������������ �����������
    pc->SomeVirtualFunction();

    return;
}


int main () {

ParentClass pc;
ChildClass cc;
ChildClass2 cc2;

    //*
    pc.ParentMethod();
    pc.SomeMethod();
    cc.ParentMethod();
    cc.SomeMethod();
    cc2.ParentMethod();
    cc2.SomeMethod();
    //*/

    // ������������ ��� ��������� ����� ParentClass
    SomeFunction (&pc);
    
    // ������ ������������� ����� ���������� ��������� �� ������ ��������� ������
    // ������������ ��� ��������� ����� ChildClass
    SomeFunction (&cc);

    SomeFunction (&cc2);


    return 0;
}
