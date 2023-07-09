#ifndef VECTORND_HPP
#define VECTORND_HPP
#define PRECISION 5

/**
 * @brief This variables will help to debug the code
 * 
 * @param {int} DEBUG : This will set the program to debug mode.
 *  1 -> true : 0 -> false
 * @param {int} IOSTREAM : This will declare iostream, sacrificing
 * time efficiency.
 *  1 -> true : 0 -> false
 */
#define DEBUG 1
#define IOSTREAM 1
#define IOSTREAM_DEBUG (DEBUG == 1 && IOSTREAM == 1)

#if IOSTREAM_DEBUG
    #include <iostream>
#endif

#include <initializer_list>
#include <stdexcept>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <string>
#include <cmath>

/**
 * @description: This function helps to speed up inverse squared root calculation.
 * Piece of code obtained in
 * https://stackoverflow.com/questions/11644441/fast-inverse-square-root-on-x64/11644533
 * 
 * @param {double} number
 */
// 
double invSqrd(double number) {
    double y = number;
    double x2 = y * 0.5;
    std::int64_t i = *(std::int64_t *) &y;
    // The magic number is for doubles is from https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
    i = 0x5fe6eb50c7b537a9 - (i >> 1);
    y = *(double *) &i;
    y = y * ( 1.5 - (x2 * y * y));   // 1st iteration
    y = y * ( 1.5 - (x2 * y * y ) );   // 2nd iteration, this can be removed
    y = y * ( 1.5 - (x2 * y * y ) );   // 2nd iteration, this can be removed
    return y;
}

class VectorND{
    protected:
        double *A = NULL;
        double norm = 0.0;
        std::uint8_t n = 0;

        double chiSqrd(void) {
            double sum = 0.0;
            for(std::uint8_t index = 0; index < this->n; ++index)
                sum += this->A[index] * this->A[index];
            return sum;
        }
        void updateNorm(void) { this->norm = sqrt(chiSqrd()); }

    public:
        void initVector(std::uint8_t N ) {
            this->n = N;
            this->A = (double *) malloc (N * sizeof(double));
        }

        VectorND(std::initializer_list<double> _list) {
            #if DEBUG == 1

            #endif
            this->initVector(_list.size() );
            for(std::uint8_t index = 0; index < _list.size(); ++index)
                this->A[index] = _list.begin()[index];
        }

        VectorND(void);
        ~VectorND(void);

        std::uint8_t dim(void) const;
        double norma(void);
        double axisAng(char,char);
        void append(double);
        double  operator[](std::uint8_t) const;
        double& operator[](std::uint8_t);
        VectorND& operator =(const VectorND&);
        VectorND& operator - (void);
        VectorND normalize(void);
        std::string getString (void);
};

#if IOSTREAM_DEBUG
    /**
     * @brief This is a debug function that will help to print easier the vectors, but sacrificing efficiency.
     * 
     * @param {VectorND} vecA
     * @example
     *  VectorND vecA ({1,2,3});
     *  std::cout << vecA << std::endl;
     * 
     * @return ostream
     */
    std::ostream& operator << (std::ostream& os, const VectorND& vecA){
        os << '<';
        for (int index = 0; index<vecA.dim(); ++index)
            (index < (vecA.dim() - 1)) ? os << vecA[index] << ',' : os << vecA[index] << '>';
        return os;
    }
#endif

VectorND::VectorND(void)
{ this->initVector(0); }

VectorND::~VectorND(void)
{  free(A);  }

double VectorND::norma(void) {
    this->updateNorm();
    return this->norm;
}

std::uint8_t VectorND::dim(void) const{ return this->n; }

double VectorND::operator [] (std::uint8_t index) const{
    index = (index<0) ? index + this->n : index;
    return this->A[index%this->n];
}

double &VectorND::operator [] (std::uint8_t index) {
    index = (index<0) ? index + this->n : index;
    return this->A[index%this->n];
}

VectorND &VectorND::operator = (const VectorND& orginal) {
    this->initVector(orginal.dim() );
    for(std::uint8_t index = 0; index<orginal.dim(); ++index)
        this->A[index] = orginal[index];

    return *this;
}

VectorND& VectorND::operator - (void) {
    for(std::uint8_t index = 0; index<this->n; ++index)
        this->A[index] *= -1;

    return *this;
}

VectorND VectorND::normalize(void) {
    VectorND norm = *this;
    double modCuad = invSqrd(chiSqrd());
    for(std::uint8_t index = 0; index<this->n; ++index)
        norm[index] *= modCuad;

    return norm;
}

/**
 * @brief This function retrieves the vector as an std::string with the following format:
 * (%.5f,%.5f,%.5f). Even though it could be easier to use iostream within the class, we
 * wont use it to make calculations a bit faster. Therefore, once you try to print the
 * object with a printf function in your main script, you'll have to use c_str ().
 * 
 * @example
 *  VectorND vec ({1,2,3});
 *  std::printf ("The vector: %s", vec.getString ().c_str ());
 * 
 * @return std::string
 */
std::string VectorND::getString (void) {
    std::string MethodName = "[Axis Angle]:";
    std::string stringVector = "(";

    for (std::uint8_t index = 0; index < this->n; ++index) {
        stringVector += (index < (this->n-1)) ? std::to_string (this->A[index]) + ',' : std::to_string (this->A[index]) + ')';
    }

    return stringVector;
}

double VectorND::axisAng(char ax1, char ax2) {
    std::uint8_t index_1, index_2;

    index_1 = (ax1 == 'X' || ax1 == 'x') ? index_1 = 0 :
        (ax1 == 'Y' || ax1 == 'y') ?  index_1 = 1 :
            (ax1 == 'Z' || ax1 == 'z') ? index_1 = 2 : -1;

    index_2 = (ax2 == 'X' || ax2 == 'x') ? index_2 = 0 :
        (ax2 == 'Y' || ax2 == 'y') ?  index_2 = 1 :
            (ax2 == 'Z' || ax2 == 'z') ? index_2 = 2 : -1;

    #if DEDBUG == 1
        std::fprintf ("===================================================\n");
        std::fprintf ("%s --> STARTED\n\n");
        std::fprintf ("First axis: %d\n", index_1);
        std::fprintf ("Second axis: %d\n", index_2);
        std::fprintf ("\n%s --> STARTED\n");
        std::fprintf ("===================================================\n\n");
    #endif

    if (index_1 == -1 || index_2 == -1) {
        std::string errorMessage = MethodName + "!!!ERROR: Vector_1 dim: " + std::to_string (vecA.dim ()) + "Vector_2 dim: " + std::to_string (vecB.dim ());
        throw std::invalid_argument ("!!!ERROR: Argument should be: x, y, z (Either Upper or Lower case)");
    }

    double x = this->A[index_1], y = this->A[index_2], angle;

    if (x > 0 && y >= 0)
        angle = atan2(y, x);
    else if (x == 0 && y > 0)
        angle = M_PI / 2;
    else if (x < 0)
        angle = atan2(y, x) + M_PI;
    else if (x == 0 && y < 0)
        angle = 3 * M_PI / 2;
    else if (x > 0 && y < 0)
        angle = atan2(y, x) + 2 * M_PI;
    else if (x == 0 && y == 0)
        angle = 0;

    return angle;
}

void VectorND::append(double r) {
    this->A = (double*) realloc (this->A, (this->n+1) * sizeof(double));
    this->A[this->n] = r;
    this->n += 1;
}

bool operator == (const VectorND& vecA, const VectorND& vecB) {
    std::string MethodName = "[Operator ==]:";
    bool result = false;

    if (vecA.dim () == vecB.dim ()) {
        result = true;
        for(uint8_t index = 0; index < vecA.dim (); ++index)
            result = result && (vecA[index] == vecB[index]);
    } else {
        std::string errorMessage = MethodName + "!!!ERROR: Vector_1 dim: " + std::to_string (vecA.dim ()) + "Vector_2 dim: " + std::to_string (vecB.dim ());
        throw std::invalid_argument (errorMessage);
    }

    return result;
}

bool operator != (const VectorND& vecA, const VectorND& vecB) { return !(vecA == vecB); }

VectorND operator + (const VectorND& vecA, const VectorND& vecB) {
    std::string MethodName = "[Operator +]:";

    VectorND out {};
    if(vecA.dim() == vecB.dim() ) {
        out.initVector(vecA.dim() );
        for(std::uint8_t index = 0; index<vecA.dim(); ++index )
            out[index] = vecA[index] + vecB[index];
    } else {
        std::string errorMessage = MethodName + "!!!ERROR: Vector_1 dim: " + std::to_string (vecA.dim ()) + "Vector_2 dim: " + std::to_string (vecB.dim ());
        throw std::invalid_argument (errorMessage);
    }

    return out;
}

VectorND operator - (const VectorND& vecA, const VectorND& vecB) {
    std::string MethodName = "[Operator -]:";

    VectorND out {};
    if(vecA.dim() == vecB.dim() ) {
        out.initVector(vecA.dim() );
        for(std::uint8_t index = 0; index<vecA.dim(); ++index )
            out[index] = vecA[index] - vecB[index];
    } else {
        std::string errorMessage = MethodName + "!!!ERROR: Vector_1 dim: " + std::to_string (vecA.dim ()) + "Vector_2 dim: " + std::to_string (vecB.dim ());
        throw std::invalid_argument (errorMessage);
    }

    return out;
}

VectorND operator * (double f, const VectorND& vecA) {
    VectorND out = vecA;

    #if DEBUG == 1
        std::cout << "double: " << f << std::endl;
        std::cout << "dim: " << vecA[0] << ',' << vecA[1] << ',' << vecA[2] << std::endl;
        std::cout << vecA << std::endl;
    #endif
    
    for(std::uint8_t index = 0; index < vecA.dim(); ++index)
        out[index] *= f;

    return out;
}

VectorND operator / (const VectorND& vecA, double f) {
    VectorND out = vecA;
    for(std::uint8_t index = 0; index<vecA.dim(); ++index)
        out[index] /= f;

    return out;
}

VectorND operator % (const VectorND& vecA, const VectorND& vecB) {
    #if IOSTREAM_DEBUG
        std::cout << std::endl;
        std::cout << "Vector A: " << vecA << std::endl;
        std::cout << "Vector B: " << vecB << std::endl;
        std::cout << std::endl;
        std::cout << "AXB[0]: vecA[1]*vecB[2] - vecA[2]*vecB[1] = " 
            << vecA[1] << '*' << vecB[2] << '-' << vecA[2] << '*' << vecB[1] << " = " 
            << vecA[1]*vecB[2] - vecA[2]*vecB[1] << std::endl;
        std::cout << "AXB[1]: vecA[2]*vecB[0] - vecA[0]*vecB[2] = " 
            << vecA[2] << '*' << vecB[0] << '-' << vecA[0] << '*' << vecB[2] << " = " 
            << vecA[2]*vecB[0] - vecA[0]*vecB[2] << std::endl;
        std::cout << "AXB[2]: vecA[0]*vecB[1] - vecA[1]*vecB[0] = " 
            << vecA[0] << '*' << vecB[1] << '-' << vecA[1] << '*' << vecB[0] << " = " 
            << vecA[0]*vecB[1] - vecA[1]*vecB[0] << std::endl;
    #endif
    std::string MethodName = "[Operator %]:";

    if (vecA.dim() != 3 || vecB.dim() != 3) {
        std::string errorMessage = MethodName + "!!!ERROR: Both vectors should be 3D";
        throw std::invalid_argument (errorMessage);
    }
    return VectorND{vecA[1]*vecB[2] - vecA[2]*vecB[1], vecA[2]*vecB[0] - vecA[0]*vecB[2], vecA[0]*vecB[1] - vecA[1]*vecB[0]};
}

double operator * (const VectorND& vecA, const VectorND& vecB) {
    std::string MethodName = "[Operator *]:";

    double out = 0.0;
    if(vecA.dim() == vecB.dim() ) {
        for(std::uint8_t index = 0; index<vecA.dim(); ++index)
            out += vecA[index] * vecB[index];
    } else {
        std::string errorMessage = MethodName + "!!!ERROR: Vector_1 dim: " + std::to_string (vecA.dim ()) + "Vector_2 dim: " + std::to_string (vecB.dim ());
        throw std::invalid_argument (errorMessage);
    }

    return out;
}

// ----- SPECIAL VECTORS --------------------

VectorND O {0,0,0};
VectorND I {1,0,0};
VectorND J {0,1,0};
VectorND K {0,0,1};
VectorND I1{1,1,1};

#endif 