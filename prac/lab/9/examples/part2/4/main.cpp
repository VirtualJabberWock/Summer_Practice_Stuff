
#include <string>
#include <vector>
#include <list>
#include <iostream>


using std::string;
using std::vector;
using std::cout;
using std::endl;


template <class AddType>
AddType Add (const AddType &lhs, const AddType &rhs) {

    return lhs + rhs;
}



void ShowString () {

string str1;
string str2("12341324123");

    str1 = "12aslkfdjaksd";

    cout << str1 << endl;
    cout << str2 << endl;
    
    string str = str1 + str2;
    cout << "str1 + str2 = " << str << endl;

    str = str1 + "aaaaaaaaaaaaaaaa";
    cout << str << endl;
    
    if (str == str1) {
        cout << "equal1\n";
        }
    if (str == str1 + "aaaaaaaaaaaaaaaa") {
        cout << "equal2\n";
        }
    
    cout << Add (str1, str2) << endl;

    cout << "01234"[1] << endl;

    cout << str[0] << endl; // operator[] (size_t index)
    
    return;
}


std::ostream & operator<< (std::ostream & stream, vector<string> &vec) {

    for (size_t i = 0; i < vec.size(); ++i) {
        // operator (ostream &, string &)
        stream << vec[i];
        stream << endl;
        }

    for (vector<string>::iterator it = vec.begin(); it < vec.end(); ++it) {
        stream << *it << endl;
        }

    return stream;
}

template <typename ElementType>
class MyIterator;

template <typename ElementType, int N = 3>
class MyContainer {

public:

    ElementType & operator[] (size_t index) {
        return buf[index];
        }

    MyIterator<ElementType> begin() {
        return MyIterator<ElementType> (*this, 0);
        }

    MyIterator<ElementType> end() {
        return MyIterator<ElementType> (*this, N);
        }


private:

    ElementType buf[N];
};

template <typename ElementType>
class MyIterator {

public:
    MyIterator (MyContainer<ElementType> &container, size_t index): container(container), index(index) {
        //this->container = container;
        }

    ElementType & operator* () const {
        return container[index];
        }

    void operator++ () {
        ++index;
        }

    bool operator == (const MyIterator &rhs) const {
        return index == rhs.index;
        }

    bool operator != (const MyIterator &rhs) const {
        return !(*this == rhs);
        }

private:

    MyContainer<ElementType> &container;
    size_t index;
};



void ShowVector () {
    
vector<int> vec1;   // int *vec = malloc()
vector<string> vec2(10);    // создаёт вектор из 10 элементов
int buf[10];
//vector< vector< vector<int> > > v;
    //v[0][0][0];

    //vec1[1];    // vec1.operator[] (1)
    buf[1];     // *(buf + 1)

    // добавить в конец
    
    vec1.push_back(10);
    cout << vec1[0] << std::endl;
    //cout << vec1[1] << std::endl;
    vec2[1] = 'c';
    vec2[9] = 'd';
    cout << vec2[1] << std::endl;
    cout << vec2[9] << std::endl;
    vec2.resize (100);
    vec2[99] = '0';
  
    vector<int> vec = vec1;
    vector<int> vec3 (vec1);
    vec3 = vec1;
    
    vec2[0] = "11111111";
    vec2[1] = "2222222222";
    vec2.resize(2);
    cout << vec2;

    return;
}


/*
void PrintContainer (int *begin, int *end) {

    for (int *p = begin; p != end; ++p) {
        std::cout << *p << std::endl;
        }

}
*/

template <typename Iter>
//[begin, end)
void PrintContainer (Iter begin, Iter end) {

    for (Iter currentIter = begin; currentIter != end; ++currentIter) {
        std::cout << *currentIter << std::endl;
        }

    return;
}


/*
template <typename Iter>
void Sort (Iter begin, Iter end) {

    Iter s = begin;
    ++begin;
    Iter n = begin;
    using std::swap;

    swap (*s, *n);
}
*/



int main () {

    //ShowString ();
    ShowVector ();

    int buf[] = {1, 2, 3};
    PrintContainer (buf, buf + sizeof(buf)/sizeof(buf[0]));

    vector<string> vecStr (3);
    vecStr[0] = "111111";
    vecStr[1] = "2222222";
    vecStr[2] = "3333333";
	PrintContainer<vector<string>::iterator> (vecStr.begin(), vecStr.end());

    string str ("abc");
    PrintContainer (str.begin(), str.end());

    std::list<char*> listStr;
    listStr.push_back ("aaaaaaa");
    listStr.push_back ("bbbbbbb");
    listStr.push_back ("ccccccc");
    PrintContainer (listStr.begin(), listStr.end());

    MyContainer<int> myCont;
    myCont[0] = 1;
    myCont[1] = 3;
    myCont[2] = 5;
    PrintContainer (myCont.begin(), myCont.end());

    return 0;
}
