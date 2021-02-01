#ifndef MATRIX3D_HPP
#define MATRIX3D_HPP

#include "VectorND.hpp"
#include <cmath>

class Matrix3D{
    private:
        double *(*M);
        int m = 4,n = 4;

    public:
        Matrix3D(double rot_th, char axis){
            this->initMatrix(1);
            rot_th *= M_PI/180;
            switch(axis){
                case 'X':
                case 'x':
                    this->M[1][1] = cos(rot_th);    this->M[1][2] =-sin(rot_th);
                    this->M[2][1] = sin(rot_th);    this->M[2][2] = cos(rot_th);
                    break;
                case 'Y':
                case 'y':
                    this->M[0][0] = cos(rot_th);    this->M[0][2] = sin(rot_th);
                    this->M[2][0] =-sin(rot_th);    this->M[2][2] = cos(rot_th);
                    break;
                case 'Z':
                case 'z':
                    this->M[0][0] = cos(rot_th);    this->M[0][1] =-sin(rot_th);
                    this->M[1][0] = sin(rot_th);    this->M[1][1] = cos(rot_th);
                    break;
                default:
                    cerr << "Really bro, you have 3 axis by the way . . .\n";
            }
        }

        double* operator [] (int i) const
        {   return this->M[i%n];   }

        void initMatrix(double trace){
            this->M = (double**) malloc (m*sizeof(double*));
            for(int i=0; i<m; ++i){
                this->M[i] = (double*) malloc (n*sizeof(double));
                M[i][i] = trace;
            }
        }
};
    
VectorND operator * (const VectorND& a, const Matrix3D& M){
    VectorND aux {0,0,0,1};
    VectorND out {0,0,0};
    for(int index=0; index<a.dim(); ++index)
        aux[index] = a[index];
    int n = 4;
    for(int i=0; i<n; ++i){
        double sum = 0.0;    
        for(int j=0; j<n; ++j)
            sum += M[i][j] * a[j]; 
        if( i!=3 )
            out[i] = sum;
    }
    return out;
}

ostream& operator << (ostream& os, const Matrix3D& M){
    for(int i=0; i<4; ++i){
        for(int j=0; j<4; ++j)
            os << M[i][j] << "\t\t";
        os << '\n';
    }
    return os;
}



#endif
