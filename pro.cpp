#include "VectorND.hpp"
#include "Matrix.hpp"

using namespace std;

int main(){
    Matrix m { VectorND{1,2},VectorND{2,3} };
    VectorND n {5,6};
    n.append(9);
    cerr <<  n << endl;
    VectorND t;
    cin >> t;
    cerr<< t;
}
