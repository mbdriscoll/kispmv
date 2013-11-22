#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>

namespace KISpMV {

template <typename ET>
struct CpuCooMatrix {
    int m, n;
    std::vector<int> rowInds;
    std::vector<int> colInds;
    std::vector<ET> vals;

    CpuCooMatrix(int m, int n,
            std::vector<int>& rowInds,
            std::vector<int>& colInds,
            std::vector<ET>& vals)
      : m(m), n(n), rowInds(rowInds), colInds(colInds), vals(vals)
    {  }

    static CpuCooMatrix<ET> CreateFromCOO(int m, int n,
            std::vector<int> rowInds,
            std::vector<int> colInds,
            std::vector<ET> vals) {
        return CpuCooMatrix(m, n, rowInds, colInds, vals);
    }
};

template<int dim>
struct CooComparator {
    std::vector<int> &rowInds, &colInds;

    CooComparator(
        std::vector<int>& rowInds,
        std::vector<int>& colInds)
      : rowInds(rowInds), colInds(colInds)
    {  }

    bool operator()(const int& i, const int& j) const {
        if (dim == 0)
            return rowInds[i] < rowInds[j];
        else if (dim == 1)
            return colInds[i] < colInds[j];
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

template <typename ET>
struct CpuCsrMatrix {
    int m, n;
    std::vector<int> rowPtrs;
    std::vector<int> colInds;
    std::vector<ET> vals;

    CpuCsrMatrix(int m, int n,
            std::vector<int>& rowPtrs,
            std::vector<int>& colInds,
            std::vector<ET>& vals)
      : m(m), n(n), rowPtrs(rowPtrs), colInds(colInds), vals(vals)
    {  }

    static CpuCsrMatrix<ET> CreateFromCOO(int m, int n,
            std::vector<int> rowInds,
            std::vector<int> colInds,
            std::vector<ET> vals) {

        // generate permutation to sort (i,j,val) tuples
        int nnz = vals.size();
        std::vector<int> perm(nnz);
        for (int i = 0; i < nnz; i++)
            perm[i] = i;

        CooComparator<1> sort_by_col(rowInds, colInds);
        CooComparator<0> sort_by_row(rowInds, colInds);

        std::stable_sort(perm.begin(), perm.end(), sort_by_col);
        std::stable_sort(perm.begin(), perm.end(), sort_by_row);

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
        // could be O(log(n)) but this implementation is O(n).
        std::vector<int> rowPtrs(m+1,0);
        for (int i = 0; i < m; i++)
            rowPtrs[i+1] = rowPtrs[i] + rowCounts[i];

        return CpuCsrMatrix(m, n, rowPtrs, colInds, vals);
    }
};

template <typename ET>
struct GpuCooMatrix {
    int m, n;
    std::vector<int> rowInds;
    std::vector<int> colInds;
    std::vector<ET> vals;

    GpuCooMatrix(int m, int n,
            std::vector<int>& rowInds,
            std::vector<int>& colInds,
            std::vector<ET>& vals)
      : m(m), n(n), rowInds(rowInds), colInds(colInds), vals(vals)
    {  }

    static GpuCooMatrix<ET> CreateFromCOO(int m, int n,
            std::vector<int> rowInds,
            std::vector<int> colInds,
            std::vector<ET> vals) {
        return GpuCooMatrix(m, n, rowInds, colInds, vals);
    }
};


template <typename VT, typename MT>
VT operator*(MT& M, VT& v);

template <typename VT, typename ET>
std::vector<VT> operator*(CpuCsrMatrix<ET>& M, std::vector<VT>& x) {
    std::cerr << "info: running cpu csr<generic> kernel" << std::endl;
    std::vector<VT> y(M.m);
    for (int i = 0; i < M.m; i++)
        for (int idx = M.rowPtrs[i]; idx < M.rowPtrs[i+1]; idx++)
            y[i] += x[M.colInds[idx]] * M.vals[idx];
    return y;
}

template <>
std::vector<double> operator*(CpuCsrMatrix<double>& M, std::vector<double>& x) {
    std::cerr << "info: running cpu csr<double> kernel" << std::endl;
    std::vector<double> y(M.m);
    for (int i = 0; i < M.m; i++)
        for (int idx = M.rowPtrs[i]; idx < M.rowPtrs[i+1]; idx++)
            y[i] += x[M.colInds[idx]] * M.vals[idx];
    return y;
}

template <typename VT, typename ET>
std::vector<VT> operator*(CpuCooMatrix<ET>& M, std::vector<VT>& x) {
    std::cerr << "info: running cpu coo kernel" << std::endl;
    std::vector<VT> y(M.m);
    for (int idx = 0; idx < M.vals.size(); idx++)
        y[M.rowInds[idx]] += x[M.colInds[idx]] * M.vals[idx];
    return y;
}

template <typename VT, typename ET>
std::vector<VT> operator*(GpuCooMatrix<ET>& M, std::vector<VT>& x) {
    std::cerr << "warning: running gpu coo kernel on cpu" << std::endl;
    std::vector<VT> y(M.m);
    for (int idx = 0; idx < M.vals.size(); idx++)
        y[M.rowInds[idx]] += x[M.colInds[idx]] * M.vals[idx];
    return y;
}

} // namespace KISpMV
