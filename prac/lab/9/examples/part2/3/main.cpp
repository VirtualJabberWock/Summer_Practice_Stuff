
#include <iostream>
#include <string>

using std::cout;


template<class T>
class SomeTemplateClass {

public:

    SomeTemplateClass (T initValue) {
        value = initValue;
        }

    T operator+ (const SomeTemplateClass &rhs) const {
        return value + rhs.value;
        }
    

    void SomeMethod ();

    template<class T2> 
    T2 SomeTemplateMethod(T2 t);

private:

    T value;
};

template<class T>
template<class T2> 
T2 SomeTemplateClass<T>::SomeTemplateMethod(T2 t) {
    
    return value + t;
}


template<class T>
void SomeTemplateClass<T>::SomeMethod() {
    
    cout << "SomeMethod\n";

    return;
}



int main () {

    SomeTemplateClass<int> sTC(10);
    SomeTemplateClass<int> sTC2(1);

    cout << sTC + sTC2 << std::endl;

    //SomeTemplateClass<double> sTC3(1);

    //cout << sTC + sTC3;
    cout << sTC.SomeTemplateMethod (2) << std::endl;
    cout << sTC.SomeTemplateMethod (2.5) << std::endl;

    return 0;
}
