
#include <iostream>

using std::cout;

#include <string>

typedef int MatrixCoeffType;

//#define max(a,b) ()
inline int max (int a, int b) {
    
    }

class Matrix {

public:
    // ��������� ����� ������ �������� �����
    // ���������� ������ (���������� ����������) ������
    void AddMatrix (Matrix &rMatrix);

    // ��������� � ���������� ����������� �� ���������
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

    // ���������� ������������
    Matrix (size_t row, size_t col);

    // ���������� � ����������� �����������
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
    // �������� ����� ������ �������� ������ �� ������� ������
    size_t colSize;
    size_t rowSize;

    MatrixCoeffType **matrix;

    // ���������� ��������� ������
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
