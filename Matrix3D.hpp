#ifndef MATRIX3D_HPP
#define MATRIX3D_HPP

#include "Matrix.hpp"

double u_nn(double u, double th)
{  return cos(th) + u*u*(1 - cos(th));  }

double u_mn(VectorND u, double th, initializer_list<int> l, int pos)
{  return u[l.begin()[0]]*u[l.begin()[1]]*(1 - cos(th)) + pos*u[l.begin()[2]]*sin(th);  }

class Matrix3D : public Matrix{
    private:
        Matrix M;
        int m = 4, n = 4;

    public:

        // Constructor:
        // Written down for specific entry to avoid to map 
        // the whole matrix (less c. time).
        Matrix3D(int trace = 1) : Matrix(4,4){
            this->A[0][0] = trace;
            this->A[1][1] = trace;
            this->A[2][2] = trace;
            this->A[3][3] = trace;
        }

        // Constructor:
        // This specific constructor generates is a rotational
        // matrix used to rotate around a vector axis.
        Matrix3D(VectorND u, double th, bool degree = 1) : Matrix(4,4){
            u = u.normalize();
            th *=  degree ? M_PI/180 : 1;
            cerr << u << endl;

            this->A[0][0] = u_nn(u[0],th)        ; this->A[0][1] = u_mn(u,th,{0,1,2},-1); this->A[0][2] = u_mn(u,th,{0,2,1},1) ;
            this->A[1][0] = u_mn(u,th,{1,0,2},1) ; this->A[1][1] = u_nn(u[1],th)        ; this->A[1][2] = u_mn(u,th,{1,2,0},-1);
            this->A[2][0] = u_mn(u,th,{2,0,1},-1); this->A[2][1] = u_mn(u,th,{2,1,0},1) ; this->A[2][2] = u_nn(u[2],th)        ;
        }

        // Constructor:
        // This is a traslation matrix
        Matrix3D(VectorND u) : Matrix(4,4){
            for(int index = 0; index<3; ++index)
                this->A[index][3] = u[index];
        }
};
    
VectorND operator * (const Matrix3D& matA, const VectorND& a){
    VectorND aux {0,0,0,1};
    VectorND out {0,0,0};
    cerr << "M------>" << endl;
    cerr << matA << endl << endl;
    cerr << "a------>" << a << endl << endl;
    for(int index=0; index<a.dim(); ++index)
        aux[index] = a[index];
    for(int i=0; i<matA.dimN(); ++i){
        double sum = 0.0;    
        for(int j=0; j<matA.dimN(); ++j){
            sum += matA[i][j] * aux[j];
            cerr << matA[i][j] << '*' << aux[j] << '=';
            cerr << sum << endl;
        }
        cerr << sum;
        cerr << endl;
        cerr << endl;

        if( i!=3 )
            cerr << out[i];
    }
    return out;
}

#endif
