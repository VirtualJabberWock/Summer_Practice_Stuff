
#include <iostream>

using std::cout;

#include <string>

typedef int MatrixCoeffType;

//#define max(a,b) ()
inline int max (int a, int b) {
    
    }

class Matrix {

public:
    // публичный члены класса доступны извне
    // объ€вление метода (публичного интерфейса) класса
    void AddMatrix (Matrix &rMatrix);

    // объ€вл€ем и определ€ем конструктор по умолчанию
    Matrix () {
        cout << this << "  Matrix()\n";
        colSize = 0;
        rowSize = 0;
        // Matrix * this;
        this->rowSize = 0;
        matrix = NULL;
        };

    Matrix & GetThisObject () {
        return *this;
        }

    void Print (std::ostream &stream) const {
        stream << "colSize = " << colSize << std::endl;
        }

    // объ€вление конструктора
    Matrix (size_t row, size_t col);

    // объ€вление и определение деструктора
    ~Matrix () {
        cout << this << "  ~Matrix()\n";
        if (matrix) {
            delete [] matrix;
            }
        }

    //*
    const Matrix & operator= (const Matrix &rhs) const {
        //matrix = rhs.matrix;
        //colSize = rhs.colSize;
        //rowSize = rhs.rowSize;
        return *this;
        }
    //*/

    friend std::ostream & operator<< (std::ostream &stream, const Matrix &matrix);

private:
    // закрытые члены класса доступны только из методов класса
    size_t colSize;
    size_t rowSize;

    MatrixCoeffType **matrix;

    // объ€вление закрытого метода
    void SomeInternalFunction ();

};

/*
Matrix M1;

    M1.AddMatrix (M2);
    M1 + M2;

void AddMatrix (Matrix *m1, Matrix *m2) {
    
    m1-> ...
}
*/
