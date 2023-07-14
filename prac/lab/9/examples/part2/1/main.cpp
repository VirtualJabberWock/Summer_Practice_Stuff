#include <iostream>

using std::cout;


class TestClass {

public:

    TestClass () {
        cout << "TestClass ()\n";
        }



    void TestClassMethod1 (/*TestClass * const this*/) {
        //TestClass * const this;

        //this = &OtherObject;
        cout << "TestClassMethod1\n";
        return;
        }


    void TestClassMethod1 () const {
        
        //const TestClass * const this;
        //a = 10; // -> this->a = 10;
        cout << "TestClassMethod1 const\n";
        return;
        }

    /*
    TestClass & operator= (const TestClass &rhs) {
        cout << "Operator=\n";
        return *this;
        }
    //*/

    //TestClass (const TestClass &ts);
    //TestClass (const TestClass &ts) {
    //    cout << "TestClass (ts)" << std::endl;
    //    };


    void TestClassMethod2 ();

private:

    // копирующий конструктор
    //TestClass (const TestClass &ts);
    //TestClass (const TestClass &ts) {};
    //TestClass (const TestClass &ts)
    //: a(ts.a), b(10)  {
    //    cout << "TestClass (TestClass &ts)\n";
    //    }

    int a;
    char b;
    char *buf;

};


inline void TestClass::TestClassMethod2() {

    cout << "TestClassMethod2" << std::endl;

    return;
}


// функция возвращает объект по значению (для этого создаётся временный объект).
TestClass FunForTestClass () {

    TestClass ts;

    return ts;      // Конструируется временный объект копирующим конструктором
}


int* fun () {

    int a;

    return &a;
}



int main () {

TestClass ts;
const TestClass &rTS = ts;
const TestClass ts2;
//TestClass &rTS2 = ts2;



    // вызов неконстантного метода для неконстантного объекта
    ts.TestClassMethod1();
    //TestClassMethod1(&ts);
    // вызов константного метода для неконстантного объектна по константной ссылке
    rTS.TestClassMethod1();
    // вызов константного метода для константного объекта
    ts2.TestClassMethod1();

    ts = FunForTestClass();
    FunForTestClass().TestClassMethod2();
    FunForTestClass().TestClassMethod1();

    TestClass ts3 = FunForTestClass();
    ts3 = FunForTestClass();

    return 0;
}
