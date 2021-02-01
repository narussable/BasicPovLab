#ifndef VECTORND_HPP
#define VECTORND_HPP

#include <initializer_list>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class VectorND{
    private:
        double  * A;
        double norm;
        int     n;

        void updateNorm(void){
            double sum = 0.0;
            for(int index=0; index<this->n; ++index)
                sum += this->A[index]*this->A[index];
            this->norm = sqrt(sum);
        }
    public:
        VectorND(initializer_list<double> _list){
            this->initVectorND(_list.size());
            for(int index=0; index<_list.size(); ++index)
                this->A[index] = _list.begin()[index];
        }

        VectorND(void)
        {  this->initVectorND(0);  }

        double norma (void){
            this->updateNorm();
            return this->norm;
        }

        int dim (void) const
        {  return this->n;  }

        void initVectorND(int n){
            this->n = n;
            this->A = (double*) malloc (n*sizeof(double));
        }

        void copyVector(const VectorND& a){
            this->initVectorND(a.dim());
            for(int index=0; index<a.dim(); ++index)
                this->A[index] = a[index];
        }

        double  operator [] (int index) const
        {   return this->A[index%this->n];   }

        double& operator [] (int index)
        {   return this->A[index%this->n];   }

        VectorND& operator = (const VectorND& orgn){
            this->initVectorND(orgn.dim());
            for(int index=0; index<orgn.dim(); ++index)
                this->A[index] = orgn[index];
            return *this;
        }
};

bool operator == (const VectorND& a, const VectorND& b){
    bool result = true;
    if(a.dim() == b.dim()){
        for(int index=0; index<a.dim(); ++index)
            result = result && (a[index]==b[index]);
        return result;
    }
    cout << "Mames bro... not the same dimension\n";
    return false;
}

VectorND operator + (const VectorND& a, const VectorND& b){
    VectorND out {};
    if( a.dim()==b.dim() ){
        out.initVectorND( a.dim() );
        for(int index=0; index<a.dim(); ++index)
            out[index] = a[index] + b[index];
    }
    else
        cerr << "You're having some dimension problems in sum . . .\n";
    return out;
}

VectorND operator - (const VectorND& a, const VectorND& b){
    VectorND out {};
    if( a.dim()==b.dim() ){
        out.initVectorND( a.dim() );
        for(int index=0; index<a.dim(); ++index)
            out[index] = a[index] - b[index];
    }
    else
        cerr << "You're having some dimension problems in sub. . .\n";
    return out;
}

VectorND operator % (const VectorND& a, const VectorND& b){
    if( a.dim()!=3 || b.dim()!=3 )
        cerr << "The cross product was realized with the first 3 entries";
    return VectorND {a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]};
} 

VectorND operator * (double r, const VectorND& b){
    VectorND out {};
    out.initVectorND(b.dim());
    for(int index=0; index<b.dim(); ++index)
        out[index] = r * b[index];
    return out;
}

VectorND operator / (const VectorND& a, double r){
    VectorND out {};
    out.initVectorND(a.dim());
    for(int index=0; index<a.dim(); ++index)
        out[index] = a[index]/r;
    return out;
}

double operator * (const VectorND& a, const VectorND& b)
{   return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];    }

ostream& operator << (ostream& os, const VectorND& a){
    os << '<';
    for(int index=0; index<a.dim(); ++index)
        ( index<(a.dim()-1) ) ? os << a[index] << ',' : os << a[index] << '>';        
    return os;
}

#endif
