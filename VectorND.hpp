#ifndef VECTORND_HPP
#define VECTORND_HPP

#include <initializer_list>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>

using namespace std;

double invSqrd(double number)
{
    double y = number;
    double x2 = y * 0.5;
    std::int64_t i = *(std::int64_t *)&y;
    // The magic number is for doubles is form
    // https://cs uwaterloo.ca/~m32rober/rsqrt.pdf
    i = 0x5fe6eb50c7b537a9 - (i >> 1);
    y = *(double *)&i;
    y = y * (1.5 - (x2 * y * y));
    y = y * (1.5 - (x2 * y * y));
    return y;
}

class VectorND
{
private:
    double *A;
    double norm;
    int n;

    double chiSqrd(void)
    {
        double sum = 0;
        for (int index = 0; index < this->n; ++index)
            sum += this->A[index] * this->A[index];
        return sum;
    }

    void updateNorm(void)
    {
        this->norm = sqrt(chiSqrd());
    }

public:
    VectorND(initializer_list<double> _list)
    {
        this->initVectorND(_list.size());
        for (int index = 0; index < _list.size(); ++index)
            this->A[index] = _list.begin()[index];
    }

    VectorND(void)
    {
        this->initVectorND(0);
    }

    double norma(void)
    {
        this->updateNorm();
        return this->norm;
    }

    int dim(void) const
    {
        return this->n;
    }

    void initVectorND(int n)
    {
        this->n = n;
        this->A = (double *)malloc(n * sizeof(double));
    }

    void copyVector(const VectorND &a)
    {
        this->initVectorND(a.dim());
        for (int index = 0; index < a.dim(); ++index)
            this->A[index] = a[index];
    }

    double operator[](int index) const
    {
        return this->A[index % this->n];
    }

    double &operator[](int index)
    {
        return this->A[index % this->n];
    }

    VectorND normalize(void)
    {
        VectorND out = *this;
        for (int index = 0; index < this->n; ++index)
            out[index] *= invSqrd(chiSqrd());
        return out;
    }

    VectorND &operator=(const VectorND &orgn)
    {
        this->initVectorND(orgn.dim());
        for (int index = 0; index < orgn.dim(); ++index)
            this->A[index] = orgn[index];
        return *this;
    }

    double axisAng(char, char);
};

double VectorND::axisAng(char ax1, char ax2)
{
    int uno, dos;

    switch (ax1)
    {
    case 'X':
    case 'x':
        uno = 0;
        break;
    case 'Y':
    case 'y':
        uno = 1;
        break;
    case 'Z':
    case 'z':
        uno = 2;
    }
    switch (ax2)
    {
    case 'X':
    case 'x':
        dos = 0;
        break;
    case 'Y':
    case 'y':
        dos = 1;
        break;
    case 'Z':
    case 'z':
        dos = 2;
    }
    double x = this->A[uno], y = this->A[dos], angle;
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

bool operator==(const VectorND &a, const VectorND &b)
{
    bool result = true;
    if (a.dim() == b.dim())
    {
        for (int index = 0; index < a.dim(); ++index)
            result = result && (a[index] == b[index]);
        return result;
    }
    cout << "Mames bro... not the same dimension\n";
    return false;
}

VectorND operator+(const VectorND &a, const VectorND &b)
{
    VectorND out{};
    if (a.dim() == b.dim())
    {
        out.initVectorND(a.dim());
        for (int index = 0; index < a.dim(); ++index)
            out[index] = a[index] + b[index];
    }
    else
        cerr << "You're having some dimension problems in sum . . .\n";
    return out;
}

VectorND operator-(const VectorND &a, const VectorND &b)
{
    VectorND out{};
    if (a.dim() == b.dim())
    {
        out.initVectorND(a.dim());
        for (int index = 0; index < a.dim(); ++index)
            out[index] = a[index] - b[index];
    }
    else
        cerr << "You're having some dimension problems in sub. . .\n";
    return out;
}

VectorND operator%(const VectorND &a, const VectorND &b)
{
    if (a.dim() != 3 || b.dim() != 3)
        cerr << "The cross product was realized with the first 3 entries";
    return VectorND{a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}

VectorND operator*(double r, const VectorND &b)
{
    VectorND out{};
    out.initVectorND(b.dim());
    for (int index = 0; index < b.dim(); ++index)
        out[index] = r * b[index];
    return out;
}

VectorND operator/(const VectorND &a, double r)
{
    VectorND out{};
    out.initVectorND(a.dim());
    for (int index = 0; index < a.dim(); ++index)
        out[index] = a[index] / r;
    return out;
}

double operator*(const VectorND &a, const VectorND &b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

ostream &operator<<(ostream &os, const VectorND &a)
{
    os << '<';
    for (int index = 0; index < a.dim(); ++index)
        (index < (a.dim() - 1)) ? os << a[index] << ',' : os << a[index] << '>';
    return os;
}

// ------- SPECIAL VECTORS POVRAY ---------------------

VectorND O{0, 0, 0};
VectorND I{1, 0, 0};
VectorND J{0, 1, 0};
VectorND K{0, 0, 1};
VectorND I1{1, 1, 1};

#endif
