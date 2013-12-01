#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>

namespace KISpMV {

struct CooComparator {
    int n;
    std::vector<int> &rowInds, &colInds;

    CooComparator(int n,
        std::vector<int>& rowInds,
        std::vector<int>& colInds)
      : n(n), rowInds(rowInds), colInds(colInds)
    {  }

    bool operator()(const int& i, const int& j) const {
        int idx0 = rowInds[i]*n + colInds[i];
        int idx1 = rowInds[j]*n + colInds[j];
        return idx0 < idx1;
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    out << "[ ";
    for (int i = 0; i < v.size(); i++)
        out << v[i] << " ";
    out << "]";
    return out;
}


template <typename ET, typename VT>
struct CpuCooMatrix;

template <typename ET, typename VT>
struct CpuCsrMatrix;

template <typename ET, typename VT>
struct Matrix {
    int m, n;

    Matrix(int m, int n)
      : m(m), n(n)
    {  }

    virtual std::vector<VT>
    operator*(std::vector<VT>& x) {
        fprintf(stderr, "No operator* defined for Matrix.\n");
        exit(2);
    }

    static Matrix<ET,VT>* CreateFromCOO(int m, int n,
            std::vector<int> rowInds,
            std::vector<int> colInds,
            std::vector<ET> vals) {

        int format_id = rand() % 2;
        switch (format_id) {
            case 1:
                return CpuCooMatrix<ET,VT>::CreateFromCOO(m, n, rowInds, colInds, vals);
            case 0:
            default:
                return CpuCsrMatrix<ET,VT>::CreateFromCOO(m, n, rowInds, colInds, vals);
        }
    }

};

template <typename MT, typename VT>
std::vector<VT> operator*(Matrix<MT,VT> *M, std::vector<VT> x) {
    return (*M) * x;
}

template <typename ET, typename VT>
struct CpuCooMatrix : public Matrix<ET,VT> {
    typedef Matrix<ET,VT> super;

    std::vector<int> rowInds;
    std::vector<int> colInds;
    std::vector<ET> vals;

    CpuCooMatrix(int m, int n,
            std::vector<int>& rowInds,
            std::vector<int>& colInds,
            std::vector<ET>& vals)
      : super(m,n), rowInds(rowInds), colInds(colInds), vals(vals)
    {  }

    static CpuCooMatrix<ET,VT>* CreateFromCOO(int m, int n,
            std::vector<int> rowInds,
            std::vector<int> colInds,
            std::vector<ET> vals) {
        return new CpuCooMatrix(m, n, rowInds, colInds, vals);
    }

    virtual std::vector<VT>
    operator*(std::vector<VT>& x) {
        std::cerr << "info: running cpu coo kernel" << std::endl;
        std::vector<VT> y(super::m, VT(0));
        for (int idx = 0; idx < vals.size(); idx++)
            y[rowInds[idx]].pluseq( x[colInds[idx]].times(x[rowInds[idx]], vals[idx]) );
        return y;
    }
};

template <typename ET, typename VT>
struct CpuCsrMatrix : public Matrix<ET,VT> {
    typedef Matrix<ET,VT> super;

    std::vector<int> rowPtrs;
    std::vector<int> colInds;
    std::vector<ET> vals;

    CpuCsrMatrix(int m, int n,
            std::vector<int>& rowPtrs,
            std::vector<int>& colInds,
            std::vector<ET>& vals)
      : super(m,n), rowPtrs(rowPtrs), colInds(colInds), vals(vals)
    {  }

    static CpuCsrMatrix<ET,VT>* CreateFromCOO(int m, int n,
            std::vector<int> rowInds,
            std::vector<int> colInds,
            std::vector<ET> vals) {

        // generate permutation to sort (i,j,val) tuples
        int nnz = vals.size();
        std::vector<int> perm(nnz);
        for (int i = 0; i < nnz; i++)
            perm[i] = i;

        CooComparator sort_by_idx(n, rowInds, colInds);
        std::sort(perm.begin(), perm.end(), sort_by_idx);

        std::vector<int> sortedRowInds(nnz);
        std::vector<int> sortedColInds(nnz);
        std::vector<ET>  sortedVals(nnz);

        // apply permutation
        for (int i = 0; i < perm.size(); i++) {
            sortedRowInds[i] = rowInds[ perm[i] ];
            sortedColInds[i] = colInds[ perm[i] ];
            sortedVals   [i] =    vals[ perm[i] ];
        }

        // count nnz per row
        std::vector<int> rowCounts(m,0);
        for (int idx = 0; idx < nnz; idx++)
            rowCounts[ sortedRowInds[idx] ] += 1;

        // prefix scan to get row offsets.
        // could be O(log(n)) but this is O(n).
        std::vector<int> rowPtrs(m+1,0);
        for (int i = 0; i < m; i++)
            rowPtrs[i+1] = rowPtrs[i] + rowCounts[i];

        return new CpuCsrMatrix(m, n, rowPtrs, sortedColInds, sortedVals);
    }

    virtual std::vector<VT>
    operator*(std::vector<VT>& x) {
        std::cerr << "info: running cpu csr<generic> kernel" << std::endl;
        std::vector<VT> y(super::m, VT(0));
        for (int i = 0; i < super::m; i++) {
            VT vtmp(0);
            for (int idx = rowPtrs[i]; idx < rowPtrs[i+1]; idx++)
                vtmp.pluseq( x[colInds[idx]].times( x[i], vals[idx] ) );
            y[i] = vtmp;
        }
        return y;
    }
};

} // namespace KISpMV
