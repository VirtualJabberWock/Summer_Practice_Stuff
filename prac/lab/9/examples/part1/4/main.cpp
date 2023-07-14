
#include <iostream>
#include "matrix.h"

#include <string>

using std::string;


using std::cout;

Matrix glM;

std::ostream & operator<< (std::ostream &stream, const Matrix &matrix) {
    
    stream << "colSize = " << matrix.colSize << std::endl;
    //matrix.Print (stream);

    return stream;
}

/*
Matrix& operator= (Matrix &matrix1, const Matrix &matrix2) {
    

    return matrix1;
}
*/


struct SomeStruct {

public:

    /*
    void InitSomeStruct (int a_, double b_) {
        a = a_;
        b = b_;
        }
    */

    void InitSomeStruct (int a_, double b_);

private:

    int a;
    double b;

};



void SomeStruct::InitSomeStruct (int a_, double b_) {
    a = a_;
    b = b_;
}


/*
void InitSomeStruct (SomeStruct *s, int a, double b) {

    s->a = a;
    s->b = b;
}
*/



int main () {

    SomeStruct s;
    s.InitSomeStruct (10, 1.0);
    s.InitSomeStruct (1, 10.0);
    //s.a = 1;
    /*
    s.a = 10;
    s.b = 1.0;
    */
    //InitSomeStruct (&s, 10, 1.0);

    /*
    Polynome pol;
    //...
    InitPolynome (&pol);
    //...
    FreePolynome (&pol);
    //*/

// конструирование локального объекта класса с конструктором по умолчанию
Matrix M1;
Matrix M2(M1);
// конструирование с передачей аргументов
Matrix M3 (10, 20);

	Matrix *pM1;
	Matrix *pM2;
	Matrix *pM3;

	try {
		pM1 = new Matrix(100, 1);
		pM2 = new Matrix();
		pM3 = new Matrix;
		throw int(10);
	} catch (int &e) {
		std::cout << "Catch except " << e << std::endl;
	}
    //operator new(sizeof(Matrix))
    //    malloc (sizeof(Matrix))
    //Matrix()


    //~Matrix();
    //operator delete(pM)
    //    free (pM)
    delete pM3;
    delete pM2;
    delete pM1;

    //M1.SetColSize (10);
    //M1.colSize = 0;
    //M1.SomeInternalFunction();

    //M1.AddMatrix (M2);
    //M2.AddMatrix (M2);
    //AddMatrix (M1, M2);
    //M2.AddMatrix (M1);


    M1 = M2 = M3;   // -> M1 = (M2 = M3)
    M1.operator = (M2.operator = (M3));
    (M2 = M3) = M1;

    std::cout << M3;


//*
    string s1 ("adadfa");
    string s2 = "aaaaaa";
    string s3;
    s3 = "adfkaj";

    std::cout << s1 + " " + s2 + "\n";
    s1 = s2;
    std::cout << s1[6] << std::endl;
    // s1[0] -> s1.operator[] (size_t index)
    std::cout << s1.at(0) << std::endl;

    if (s1 == "-n") {
        
        }
    if (s1 == s2) {
        
        }

    printf ("c_str: %p  data: %p\n", s1.c_str(), s1.data());
    ((char*)s1.data())[6] = 'b';
    ((char*)s1.data())[7] = 'c';
    const char *p = s1.c_str();
    std::cout << "char*: " << p << std::endl;
    std::cout << s1.data() << std::endl;
//*/
    return 0;
}


