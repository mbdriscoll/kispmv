#include <iostream>
#include <vector>

namespace KISpMV {

// --------------------------------------------------------------------------
// forward declaration

template <typename elt_t> class CpuCsrMatrix;
template <typename elt_t> class CpuCooMatrix;


// --------------------------------------------------------------------------
// main matrix classes

template <typename elt_t>
class Vector : std::vector<elt_t> {
    typedef std:;vector<elt_t> super;

    Vector(int nElems)
      : super(nElems)
    { }
};

template <typename elt_t>
class Matrix {

  protected:

    int m, n;

    Matrix(int m, int n)
      : m(m), n(n)
    { }

  public:

    virtual int nnz() = 0;

    // factory to create matrices
    static Matrix<elt_t>* CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
    {
        if (1) {
            return CpuCsrMatrix<elt_t>::CreateFromCSR(m,n,rowPtrs,colInds,vals);;
        } else {
            return CpuCooMatrix<elt_t>::CreateFromCSR(m,n,rowPtrs,colInds,vals);;
        }
    }

    // factory to create matrices
    static Matrix<elt_t>* CreateFromDense(int m, int n,
        std::vector<int> &values)
    {
    }
};

// --------------------------------------------------------------------------
// concrete matrix classes

template <typename elt_t>
class CpuCsrMatrix : Matrix<elt_t> {

    typedef Matrix<elt_t> super;

    std::vector<int>   &rowPtrs,
                       &colInds;
    std::vector<elt_t> &vals;

    CpuCsrMatrix(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
      : super(m,n), rowPtrs(rowPtrs), colInds(colInds), vals(vals) {
          std::cout << "Made CpuCsrMatrix" << std::endl;
    }

  public:

    virtual int nnz() {
        return rowPtrs[super::m];
    }

    static Matrix<elt_t>* CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
    {
        return (Matrix<elt_t>*) new CpuCsrMatrix<elt_t>(m, n, rowPtrs, colInds, vals);
    }
};

template <typename elt_t>
class CpuCooMatrix : Matrix<elt_t> {

    typedef Matrix<elt_t> super;

    std::vector<int>   &rowInds,
                       &colInds;
    std::vector<elt_t> &vals;

    CpuCooMatrix(int m, int n,
        std::vector<int> &rowInds,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
      : super(m,n), rowInds(rowInds), colInds(colInds), vals(vals) {
          std::cout << "Made CpuCooMatrix" << std::endl;
    }

  public:

    virtual int nnz() {
        return rowInds.size();
    }

    static Matrix<elt_t>* CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
    {
        return (Matrix<elt_t>*) new CpuCooMatrix<elt_t>(m, n, rowPtrs, colInds, vals);
    }
};

} /* namespace KISpMV */
