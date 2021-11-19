#ifndef BMP2MAT_HPP
#define BMP2MAT_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Matrix.hpp"

class BoolMat{
    protected:
        std::ifstream file;
        Matrix           G;
        const char  * File;
        unsigned int   * M;
        unsigned int dim = 0;
        int density = 3;
        int m = 0 ,n = 0;

    public:
        double  size;
        double width;
        const char *  surf;

        // General constructor ------------------------------------------------------------------
        BoolMat(const char * file_name){
            this->file = std::ifstream(file_name);
            this->File = file_name;
            this->M = (unsigned int*) malloc (0*sizeof(unsigned int));
            this->scanFile();
            this->n = this->dim/this->m;
            this->G = Matrix(this->m,this->n);
            this->fillMatrix();
            std::cout << this->m << ' '  << this->n << "\n";
        }

        // Constructor specialized in Povray ------------------------------------------------------
        BoolMat(const char * file_name, double si, double w, const char * s){
            this-> size = si;
            this-> width= w;
            this->surf  = s;
            this->file = std::ifstream(file_name);
            this->File = file_name;
            this->M = (unsigned int*) malloc (0*sizeof(unsigned int));
            this->scanFile();
            this->n = this->dim/this->m;
            this->G = Matrix(this->m,this->n);
            this->fillMatrix();
            std::cout << this->m << ' '  << this->n << "\n";
        }

        void pushM(unsigned int new_value){
            this->M = (unsigned int*) realloc (this->M,(this->dim+1)*sizeof(unsigned int));
            this->M[this->dim] = new_value;
            ++this->dim;
        }

        void resetArray(char d_array[] ){
            delete [] d_array;
            d_array = (char*) malloc (this->density*sizeof(char));
        }

        void scanFile(void){
            this->file.clear();
            this->file.seekg(0);
            unsigned int count = 0;
            char * digit = (char*) malloc (this->density*sizeof(char));
            while( this->file.peek()!=EOF ){
                char Lex = this->file.get();
                bool isDigit = (Lex!=' ') && (Lex!='\n') && this->file.peek()!=EOF;
                if(isDigit){
                    digit[count] = Lex;
                    ++count;
                }
                if(Lex==' ' || Lex=='\n'){
                    this->pushM( atoi(digit) );
                    this->resetArray(digit);
                    count = 0;
                }
                if(Lex=='\n') ++this->m;
            }
        }

        void fillMatrix(void){
            unsigned int count = 0;
            for(int i=0; i<this->m; ++i){
                for(int j=0; j<this->n; ++j){
                    this->G[i][j] = M[count];
                    ++count;
                }
            }
        }

        int getM(void) const
        {  return this->m;   }

        int getN(void) const
        {  return this->n;   }

        double* operator [] (int index)
        {  return G[index];  }

        void copyBool(BoolMat& m){
            for(int i=0; i<m.getM(); ++i){
                for(int j=0; j<m.getN(); ++j)
                    this->G[i][j] = m[i][j];
            }
        }
};

#endif
