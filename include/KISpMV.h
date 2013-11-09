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
class Vector : public std::vector<elt_t> {
    typedef std::vector<elt_t> super;

  public:
    Vector(int nElems, elt_t elt = elt_t())
      : super(nElems)
    { }
};

template <typename elt_t>
class Matrix {

  protected:

    int m, n;
    std::vector<int>   rowPtrs,
                       colInds;
    std::vector<elt_t> vals;

    Matrix(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
      : m(m), n(n), rowPtrs(rowPtrs), colInds(colInds), vals(vals)
    { }

  public:

    template <typename x_elt_t>
    Vector<x_elt_t> operator*(Vector<x_elt_t> x) {
        Vector<x_elt_t> y(m, x_elt_t());
        for (int i = 0; i < m; i++) {
            for (int idx = rowPtrs[i]; idx < rowPtrs[i+1]; idx++) {
                y[i] += vals[idx] * x[colInds[idx]];
            }
        }
        return y;
    }

    int nnz() {
        return rowPtrs[m];
    }

    // factory to create matrices
    static Matrix<elt_t> CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrsSrc,
        std::vector<int> &colIndsSrc,
        std::vector<elt_t> &valsSrc)
    {
        std::vector<int> *rowPtrs = new std::vector<int>(rowPtrsSrc);
        std::vector<int> *colInds = new std::vector<int>(colIndsSrc);
        std::vector<elt_t> *vals  = new std::vector<elt_t>(valsSrc);
        return Matrix(m, n, *rowPtrs, *colInds, *vals);
    }
};

} /* namespace KISpMV */
