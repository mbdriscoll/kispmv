#include <cstdlib>
#include <vector>

namespace KISpMV {

template <typename ET>
struct CooMatrix {
    int m, n;
    std::vector<int> rowInds;
    std::vector<int> colInds;
    std::vector<ET> vals;

    CooMatrix(int m, int n,
            std::vector<int>& rowInds,
            std::vector<int>& colInds,
            std::vector<ET>& vals)
      : m(m), n(n), rowInds(rowInds), colInds(colInds), vals(vals)
    {  }

    static CooMatrix<ET> CreateFromCOO(int m, int n,
            std::vector<int> rowInds,
            std::vector<int> colInds,
            std::vector<ET> vals) {
        return CooMatrix(m, n, rowInds, colInds, vals);
    }
};

template <typename ET>
struct CsrMatrix {
    int m, n;
    std::vector<int> rowPtrs;
    std::vector<int> colInds;
    std::vector<ET> vals;

    CsrMatrix(int m, int n,
            std::vector<int>& rowPtrs,
            std::vector<int>& colInds,
            std::vector<ET>& vals)
      : m(m), n(n), rowPtrs(rowPtrs), colInds(colInds), vals(vals)
    {  }

    static CsrMatrix<ET> CreateFromCOO(int m, int n,
            std::vector<int> rowInds,
            std::vector<int> colInds,
            std::vector<ET> vals) {
        // TODO fancy sorting magic
        return CsrMatrix(m, n, rowInds, colInds, vals);
    }
};

template <typename VT, typename MT>
VT operator*(MT& M, VT& v);

template <typename VT, typename ET>
std::vector<VT> operator*(CsrMatrix<ET>& M, std::vector<VT>& x) {
    std::vector<VT> y(M.m);
    for (int i = 0; i < M.m; i++)
        for (int idx = M.rowPtrs[i]; idx < M.rowPtrs[i+1]; idx++)
            y[i] += x[M.colInds[idx]] * M.vals[idx];
    return y;
}

template <>
std::vector<double> operator*(CsrMatrix<double>& M, std::vector<double>& x) {
    std::vector<double> y(M.m);
    for (int i = 0; i < M.m; i++)
        for (int idx = M.rowPtrs[i]; idx < M.rowPtrs[i+1]; idx++)
            y[i] += x[M.colInds[idx]] * M.vals[idx];
    return y;
}

template <typename VT, typename ET>
std::vector<VT> operator*(CooMatrix<ET>& M, std::vector<VT>& x) {
    std::vector<VT> y(M.m);
    for (int idx = 0; idx < M.vals.size(); idx++)
        y[M.rowInds[idx]] += x[M.colInds[idx]] * M.vals[idx];
    return y;
}

} // namespace KISpMV
