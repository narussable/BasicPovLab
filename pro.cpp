#include "VectorND.hpp"
#include "Matrix.hpp"

using namespace std;

int main(){
    Matrix m { VectorND{1,2,3,4,5,6,7},VectorND{2,3,4,5,6,7,8} };
    cerr << m << endl;
}
