#include <cstdio>
#include <KISpMV.h>

int main(int argc, char *argv[]) {

    int nTrials = 10, seed = 13;
    int m = 100, n = 50, nnz = 200;
    std::vector<int> rowInds(nnz),
                     colInds(nnz);
    std::vector<double> vals(nnz);
    std::vector<double> x(n, 1.0);

    srand48(seed);
    for (int i = 0; i < nnz; i++) {
        rowInds[i] = rand() % m;
        colInds[i] = rand() % n;
        vals[i] = drand48();
    }

    KISpMV::CsrMatrix<double> M_csr = KISpMV::CsrMatrix<double>::CreateFromCOO(m, n, rowInds, colInds, vals);
    KISpMV::CooMatrix<double> M_coo = KISpMV::CooMatrix<double>::CreateFromCOO(m, n, rowInds, colInds, vals);

    std::vector<double> y_csr = M_csr * x;
    std::vector<double> y_coo = M_coo * x;

    return 0;
}
