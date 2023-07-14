#include <iostream>
#include "matrix.h"

using std::cout;


// определение метода класса
//Matrix::AddMatrix (Matrix * const this, Matrix &rMatrix)
void Matrix::AddMatrix (Matrix &rMatrix) {

    rMatrix.rowSize = 10;
    rowSize = 10;   // = this->rowSize = 10

    return;
}

Matrix::Matrix (size_t row, size_t col) {

    cout << this << "  Matrix (size_t, size_t)\n";

    colSize = col;
    rowSize = row;

    matrix = new MatrixCoeffType*[row];
}


//struct Polynom;
//
//
//Polynom pol;
//
//    InitPolynom (pol, 10);
//    ...
//    ClearPolynom (pol);
