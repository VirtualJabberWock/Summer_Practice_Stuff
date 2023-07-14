#include <iostream>


namespace NS {

}

// ....

namespace NS {

}

// так может быть плохо
using namespace std;


// так может быть лучше
using std::cout;
using std::cin;
using std::endl;

struct TestStruct {
 
    int a;
    int b;
};



//*
std::ostream & operator<< (std::ostream &stream, const TestStruct &ts) {

    stream << "a = " << ts.a << " b = " << ts.b << endl;
	//stream << ts;
    return stream;
}
//*/



int main () {

    int a;
    int b;

    //printf ("%d %s\n", 1, "Hello, World!");
	std::cout << 1 << "Hello" << std::endl;
    std::cout << "Hello, World!" << std::endl;
    cout << "a = ";
    cin >> a;
    //scanf ("%d", &a);

    cout << "a = " << a + 1 << endl;
    // ((cout << "a = ") << a) << endl;

	TestStruct s1, s2;

	//AddStruct (&s1, &s2);
	//s1 + s2;

    TestStruct ts = {10, 20};
    cout << ts;
    operator<< (cout, ts);
    //PrintTestStruct (cout, ts);
    //a << b;

    //10 + 10
    //operator+ (10, 10);
    //int operator+ (int, int)
    //operator+(1, 2);
    //ts + 10
    //10 + ts

    return 0;
}
