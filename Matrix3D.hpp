#ifndef MATRIX3D_HPP
#define MATRIX3D_HPP

#include "VectorND.hpp"
#include <cmath>

class Matrix3D{
    private:
        double *(*M);
        int m = 4,n = 4;

    public:
        Matrix3D(void)
        { this->initMatrix(1); }

        Matrix3D (double scl)
        { this->initMatrix(scl); }

        Matrix3D (VectorND tras){
            this->initMatrix(1);
            for(int index=0; index<3; ++index)
                this->M[index][3] = tras[index];
        }

        Matrix3D (VectorND v, double th){
            this->initMatrix(1);
            th *= M_PI/180;

            double x = v[0], y = v[1], z = v[2];

            double umc = 1 - cos(th);
            double co = cos(th);
            double si = sin(th); 

            this->M[0][0] = co + x*x*umc  ; this->M[1][0] = x*y*umc - z*si; this->M[2][0] = x*z*umc + y*si;
            this->M[0][1] = y*x*umc + z*si; this->M[1][1] = co + y*y*umc  ; this->M[2][1] = y*z*umc - x*si;
            this->M[0][2] = z*x*umc - y*si; this->M[1][2] = z*y*umc + x*si; this->M[2][2] = co + z*z*umc  ;
        }

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
    
VectorND operator * (const Matrix3D& M, const VectorND& a){
    VectorND aux {0,0,0,1};
    VectorND out {0,0,0};
    for(int index=0; index<a.dim(); ++index)
        aux[index] = a[index];
    int n = 4;
    for(int i=0; i<n; ++i){
        double sum = 0.0;    
        for(int j=0; j<n; ++j)
            sum += M[i][j] * aux[j];
        if( i!=3 )
            out[i] = sum;
    }
    return out;
}

Matrix3D operator * (const Matrix3D& M, const Matrix3D& N){
    Matrix3D out;
    for(int i=0; i<4; ++i){
        for(int j=0; j<4; ++j){
            double sum = 0.0;
            for(int k=0; k<4; ++k)
                sum += M[i][k]*N[k][j];
            out[i][j] = sum;
        }
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
