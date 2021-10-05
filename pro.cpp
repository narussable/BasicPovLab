#include "VectorND.hpp"
#include "Matrix.hpp"

using namespace std;

int main(){
    Matrix m { VectorND{1,2,3,4,5,6,7},VectorND{2,3,4,5,6,7,8} };
    cerr << m << endl << endl;

    cerr << -m << endl;
    VectorND aa {1,2,-2};
    VectorND bb {1,5,-4};
    cerr << aa << endl;
    cerr << bb << endl;
    cerr << aa-bb << endl;
}
