#include <iostream>
#include <vector>

namespace KISpMV {

template <typename elttype, typename idxtype = int>
class Vector : public std::vector<elttype> {
    typedef std::vector<elttype> super;

    void eval() {
        std::cout << "Eval V" << std::endl;
    }

public:
    Vector(int nElts) : super(nElts) {
    }

    Vector& operator=(Vector<elttype> o) {
        std::cout << "Assign!" << std::endl;
        return *this;
    }

    elttype& operator[](idxtype i) {
        std::cout << "Access!" << std::endl;
        this->eval();
        return this->super::operator[](i);
    }
};

template <typename ytype,
          typename mtype,
          typename xtype,
          typename idxtype = int>
class Matrix {

    idxtype m, n;

    Matrix(idxtype m, idxtype n)
        : m(m), n(n)
    { }

    void eval() {
        std::cout << "Eval M" << std::endl;
    }

public:
    static Matrix
    CreateFromCSR(idxtype m, idxtype n, idxtype *rowPtrs, idxtype *colInds, mtype *vals) {
        return Matrix(m, n);
    }

    Vector<ytype> operator*(std::vector<xtype>& x) {
        std::cout << "Multiply MV!" << std::endl;
        return Vector<ytype>(m);
    }

    Matrix operator*(Matrix<ytype,mtype,xtype,idxtype> o) {
        std::cout << "Multiply MM!" << std::endl;
        return Matrix(m,n);
    }
};

} /* namespace KISpMV */
