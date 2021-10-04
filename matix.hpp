#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <cstdlib>
#include <cmath>

class MatrixInt {
    public:
    	int m, n;
    	int * * A;

        void initMatrixInt(int, int);
		void destroyMatrixInt();
        void zeroMatrixInt(int, int);
        int getM();
        int getN();
        int getA(int, int);
        void updateA(int, int, int);
        void makeEqualMatrixInt(MatrixInt);
        void escMatrixInt();
        void escMatrixInt0();

        int* operator [] (int index)
        {   return this->A[index%this->m];   }
};


void MatrixInt::initMatrixInt(int m, int n) {

	this->m = m;
	this->n = n;
	this->A = (int **) malloc (m * sizeof(int*));
	for (int i = 0; i < m; i++)
		this->A[i] = (int *) malloc (n * sizeof(int));

}

void MatrixInt::destroyMatrixInt() {

	for (int i = 0; i < this->m; i++) {
		delete this->A[i];
		this->A[i] = NULL;
	}

	delete this->A;
	this->A = NULL;

	this->m = 0;
	this->n = 0;
}

int MatrixInt::getM() {
	return this->m;
}

int MatrixInt::getN() {
	return this->n;
}

int MatrixInt::getA(int i, int j) {
	return this->A[i][j];
}

void MatrixInt::updateA(int i, int j, int x) {
	this->A[i][j] = x;
}

void MatrixInt::makeEqualMatrixInt(MatrixInt mat) {

	if (this->m == mat.getM() && this->n == mat.getN()) {

		for (int i = 0; i < this->m; i++)
			for (int j = 0; j < this->n; j++)
				this->updateA(i, j, mat.getA(i, j));
	} else {
		printf("\n\n--> In makeEqualMatrixInt :: unable to make equal\n--> Please check dimensions\n\n");
	}
}

void MatrixInt::zeroMatrixInt(int m, int n) {

	this->initMatrixInt(m, n);
	for (int i = 0; i < this->m; ++i)
		for (int j = 0; j < this->n; ++j)
			this->A[i][j] = 0;
}

void MatrixInt::escMatrixInt() {

	for (int i = 0; i < this->m; i++) {
		for (int j = 0; j < this->n; j++)
			printf("   %d", this->getA(i, j));
		printf("\n\n");
	}
}

void MatrixInt::escMatrixInt0() {
	printf("\n");
	for (int i = 0; i < this->m; i++) {
		for (int j = 0; j < this->n; j++)
			printf("   %d", this->getA(i, j));
	}
	printf("\n");
}

#endif
