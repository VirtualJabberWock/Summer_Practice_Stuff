#include <iostream>

using std::cout;


//typedef int AddType;


class AddClass {
  
public:

    AddClass () {
        }

    AddClass (int initValue) {
        value = initValue;
        }

    const AddClass operator+ (const AddClass &rhs) {
        return AddClass (value + rhs.value);
        }

    friend std::ostream & operator << (std::ostream &stream, const AddClass &ac);

private:
    int value;
    
};


std::ostream & operator << (std::ostream &stream, const AddClass &ac) {

    stream << ac.value << std::endl;
    
    return stream;
}


template<class AddType>
AddType Add (AddType a, AddType b) {

	cout << "Template Add\n";
    // умеет складывать любые типы, для которых возможна операция operator+
    return a + b;
}


//int Add (int a, int b) {
//
//    cout << "Add\n";
//    // умеет складывать любые типы, для которых возможна операция operator+
//    return a + b;
//}


//AddClass Add (AddClass &a, AddClass &b) {
//
//    return a + b;
//}



template<class AddType1, class AddType2>
AddType2 Add2 (AddType1 a, AddType2 b) {

	cout << "Add2\n";

    return a + b;
}


int main () {

    cout << Add (10, 20) << std::endl;
    // int Add (int, int) автоматически
    cout << Add ((char)(256 + 10), (char)(256 + 20)) << std::endl;
    
    cout << Add (1.5, 2.5) << std::endl;
    // double Add (double, double) автоматически

    cout << Add<int> (1, 1.5) << std::endl;
	// int Add (int, int) указано нами

    //cout << Add (1, 1.5) << std::endl;
    
    cout << Add2 (1, 1.5) << std::endl;

    AddClass ac1(10);
    AddClass ac2(20);

    //cout << ac1 + ac2;

    cout << Add (ac1, ac2);

    return 0;
}
