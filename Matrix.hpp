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

        bool dimComparation(const Matrix&) const;
        unsigned int dimM(void) const;
        unsigned int dimN(void) const;
        void initMatrix(int=0,int=0);
        double* operator [] (int) const;
        Matrix& operator = (const Matrix&);
        Matrix& operator - (void);
};

void Matrix::initMatrix(int m, int n){
    this->m = m;
    this->n = n;
    this->A = (double**) malloc (m * sizeof(double*));
    for(int index = 0; index<m; ++index)
        this->A[index] = (double*) malloc (n* sizeof(double)); 
}

bool Matrix::dimComparation(const Matrix& matA) const
{ return ( this->m == matA.dimM() ) && ( this->n == matA.dimN() ); }

unsigned int Matrix::dimM(void) const{ return this->m; }
unsigned int Matrix::dimN(void) const{ return this->n; }
double* Matrix::operator [] (int index) const { return this->A[index%this->n]; }

Matrix& Matrix::operator = (const Matrix& matA){
    this->initMatrix( matA.dimM(), matA.dimN() );
    for(int i = 0; i<matA.dimM(); ++i){
        for(int j = 0; j<matA.dimN(); ++j)
            this->A[i][j] = matA[i][j];
    }
    return *this;
}

Matrix& Matrix::operator - (void){
    for(int i = 0; i<this->m; ++i){
        for(int j = 0; j<this->n; ++j)
            this->A[i][j] *= -1;
    }
    return *this;
}

Matrix operator + (const Matrix& matA,const Matrix& matB){
    Matrix out(0,0);
    if( matA.dimComparation(matB) ){
        out = matA;
        for(int i = 0; i<matA.dimM(); ++i){
            for(int j = 0; j<matA.dimN(); ++j)
                out[i][j] += matB[i][j];
        }
    }else
        cerr << "Dimensional problem in matrix operation +" << endl;
    return out;    
}

Matrix operator * (const Matrix& matA, const Matrix& matB){
    Matrix out (0,0);
    if( matA.dimN()==matB.dimM() ){
        out.initMatrix( matA.dimM(), matB.dimN() );
        for(int i = 0; i<matA.dimM(); ++i){
            for(int j = 0; j<matB.dimN(); ++j){
                double suma = 0.0;
                for(int k = 0; k<matB.dimM(); ++k)
                    suma += matA[i][k] * matB[k][j];
                out[i][j] = suma; 
            }
        }
    }else
        cerr << "Dimensional problem in matrix operation *" << endl;
    return out;
}


ostream& operator << (ostream& os, const Matrix& M){
    for(int i=0; i<M.dimM(); ++i){
        for(int j=0; j<M.dimN(); ++j)
            os << M[i][j] << "\t\t";
       os << '\n';
    }
    return os;
}

VectorND operator * (const Matrix& matA, const VectorND& vecB){
    VectorND out {};
    if( matA.dimM()==vecB.dim() ){
        out = vecB;
        Matrix aux( vecB.dim(),1 );
        for(int index = 0; index<vecB.dim(); ++index)
            aux[index][0] = vecB[index];
        cerr << aux << endl;
        //aux = matA * aux;
        //for(int index = 0; index<vecB.dim(); ++index)
        //    out[index] = aux[index][0];
    }else
        cerr << "Dimensional probelm in matrix operation * (vector)" << endl;
    return out;
}

#endif
