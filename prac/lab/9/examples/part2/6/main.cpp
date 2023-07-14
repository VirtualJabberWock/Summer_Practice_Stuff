#include <iostream>
#include <string>

using std::cout;

// абстрактный класс (т.к. содержит чисто виртуальные методы)
// будет использоваться только для наследования
class MatrixValue {

public:

    // чисто виртуальные методы без определения
    virtual void Add (MatrixValue & rhs) = 0;

    virtual MatrixValue & operator+ (MatrixValue & rhs) = 0;

    virtual void PrintValue (std::ostream &stream) = 0;

    // при использовании динамического полиморфизма
    // деструктор следует делать виртуальным
    virtual ~MatrixValue() {};

    virtual MatrixValue * Copy () = 0;

};



std::ostream & operator<< (std::ostream & stream, MatrixValue & mvi) {

    mvi.PrintValue (stream);

    return stream;
}


template<class StdType>
class MatrixValueStd: public MatrixValue {

public:
    virtual void Add (MatrixValue & rhs) {
        value += (static_cast<MatrixValueStd*>(&rhs))->value;
        };

    MatrixValueStd (StdType initValue) {
        value = initValue;
        }

    virtual ~MatrixValueStd() {};

    // виртуальный конструктор копирования
    virtual MatrixValueStd * Copy () {
        return new MatrixValueStd (value);
        }
    
    // перегруженный виртуальный метод может возвращать ссылку на объект
    // производного класса, если в родительском этот метод возвращает
    // ссылку на объект родительского класса
    virtual MatrixValueStd & operator+ (MatrixValue & rhs) {
        return *new MatrixValueStd (value + static_cast<MatrixValueStd&>(rhs).value);
        }

    virtual void PrintValue (std::ostream &stream) {
        stream << value;
        }

private:

    StdType value;

};


class Matrix {

public:

    Matrix () {
        colSize = 0;
        rowSize = 0;
        matrix = NULL;
        }

    Matrix (size_t colInit, size_t rowInit) {
        AllocMatrix (colInit, rowInit);
        }

    Matrix (const Matrix &rhs) {
        colSize = 0;
        rowSize = 0;
        matrix = NULL;
        *this = rhs;
        }

    Matrix & operator+= (const Matrix & rhs) {

        for (size_t i = 0; i < rowSize; ++i) {
            for (size_t j = 0; j < colSize; ++j) {
                (matrix[i][j])->Add (*rhs.matrix[i][j]);
                }
            }

        return *this;
        }

    Matrix & operator= (const Matrix & rhs) {

        FreeMatrix();

        colSize = rhs.colSize;
        rowSize = rhs.rowSize;

        matrix = new MatrixValue**[rowSize];
        for (size_t i = 0; i < rowSize; ++i) {
            matrix[i] = new MatrixValue*[colSize];
            for (size_t j = 0; j < colSize; ++j) {
                matrix[i][j] = (rhs.matrix[i][j])->Copy();
                }
            }

        return *this;
        }

    ~Matrix () {
        FreeMatrix();
        }

    Matrix operator+ (const Matrix & rhs) {

        Matrix res (colSize, rowSize);

        for (size_t i = 0; i < rowSize; ++i) {
            for (size_t j = 0; j < colSize; ++j) {
                res.matrix[i][j] = &(*matrix[i][j] + *rhs.matrix[i][j]);
                }
            }

        return res;
        }

    template<class ValueType, class Iter>
    void Init (size_t colInit, size_t rowInit, Iter begin, Iter end) {

        FreeMatrix();

        AllocMatrix (colInit, rowInit);

        Iter current = begin;
        for (size_t i = 0; i < rowSize; ++i) {
            for (size_t j = 0; j < colSize; ++j) {
                if (current == end)
                    return;
                matrix[i][j] = new MatrixValueStd<ValueType> (*(current++));
                }
            }
        }

    friend std::ostream & operator<< (std::ostream & stream, Matrix &matrix);

private:

    void FreeMatrix () {
        for (size_t i = 0; i < rowSize; ++i) {
            for (size_t j = 0; j < colSize; ++j) {
                delete matrix[i][j];
                }
            delete [] matrix[i];
            }
        delete [] matrix;
        }

    void AllocMatrix (size_t colInit, size_t rowInit) {
        colSize = colInit;
        rowSize = rowInit;
        matrix = new MatrixValue**[rowSize];
        for (size_t i = 0; i < rowSize; ++i) {
            matrix[i] = new MatrixValue*[colSize];
            }
        }
    

    size_t rowSize;
    size_t colSize;

    MatrixValue ***matrix;

};


std::ostream & operator<< (std::ostream & stream, Matrix &matrix) {
    
    for (size_t i = 0; i < matrix.rowSize; ++i) {
        for (size_t j = 0; j < matrix.colSize; ++j) {
            cout << *matrix.matrix[i][j] << " ";
            }
        cout << std::endl;
        }

    return stream;
}


int main () {

const size_t rowSize = 1;
const size_t colSize = 1;
Matrix m1;
Matrix m2;
Matrix res;


    char *m1ValuesChar[] = {"aaaaaaaaaa", "bbbbbbbbbbbb", "cccccccccc", "dddddddddd"};
    char *m2ValuesChar[] = {"1111111111", "222222222222", "3333333333", "4444444444"};
    m1.Init<std::string> (2, 2, m1ValuesChar, m1ValuesChar + 4);
    m2.Init<std::string> (2, 2, m2ValuesChar, m2ValuesChar + 4);

    //m1.matrix[0][0] = new MatrixValueStd<std::string> ("aaaaaaa");
    //m2.matrix[0][0] = new MatrixValueStd<std::string> ("bbbbbbb");
    m1 += m2;
    cout << m1;
    res = m1 + m2;
    cout << res;

    int m1ValuesInt[] = {1, 2, 3, 4};
    int m2ValuesInt[] = {10, 20, 30, 40};
    m1.Init<int> (2, 2, m1ValuesInt, m1ValuesInt + 4);
    m2.Init<int> (2, 2, m2ValuesInt, m2ValuesInt + 4);
    //m1.matrix[0][0] = new MatrixValueStd<int> (10);
    //m2.matrix[0][0] = new MatrixValueStd<int> (20);
    m1 += m2;
    cout << m1;
    res = m1 + m2;
    cout << res;
    
    double m1ValuesDouble[] = {1.01, 2.02, 3.03, 4.04};
    double m2ValuesDouble[] = {10.10, 20.20, 30.30, 40.40};
    m1.Init<double> (2, 2, m1ValuesDouble, m1ValuesDouble + 4);
    m2.Init<double> (2, 2, m2ValuesDouble, m2ValuesDouble + 4);
    //m1.matrix[0][0] = new MatrixValueStd<double> (10.1234);
    //m2.matrix[0][0] = new MatrixValueStd<double> (20.1234);
    m1 += m2;
    cout << m1;
    res = m1 + m2;
    cout << res;

    return 0;
}
