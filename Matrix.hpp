#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "VectorND.hpp"

using namespace std;

class Matrix{
    protected:
        double **A;
        int    m,n;
    public:
        Matrix(int m = 0,int n = 0){ this->initMatrix(m,n); }

        Matrix(initializer_list<VectorND> row){
            this->initMatrix( row.size(), row.begin()[0].dim() );
            for(int i = 0; i<this->m; ++i){
                for(int j = 0; j<this->n; ++j)
                    this->A[i][j] = row.begin()[i][j];
            }
        }

        unsigned int dimM(void) const;
        unsigned int dimN(void) const;
        void initMatrix(int=0,int=0);
        double* operator [] (int) const;
};

void Matrix::initMatrix(int m, int n){
    this->m = m;
    this->n = n;
    this->A = (double**) malloc (m * sizeof(double*));
    for(int index = 0; index<m; ++index)
        this->A[index] = (double*) malloc (n* sizeof(double)); 
}

double* Matrix::operator [] (int index) const { return this->A[index%this->n]; }

unsigned int Matrix::dimM(void) const{ return this->m; }
unsigned int Matrix::dimN(void) const{ return this->n; }

ostream& operator << (ostream& os, const Matrix& M){
    for(int i=0; i<M.dimM(); ++i){
        for(int j=0; j<M.dimN(); ++j)
            os << M[i][j] << "\t\t";
       os << '\n';
    }
    return os;
}

#endif
