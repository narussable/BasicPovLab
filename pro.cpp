#include "VectorND.hpp"
#include "Matrix.hpp"

using namespace std;

int main(){
    Matrix m { VectorND{1,2},VectorND{2,3} };
    VectorND n {5,6};
    cerr << m << endl;
    cerr << n << endl << endl;
    cerr << m*n << endl << endl;
}
