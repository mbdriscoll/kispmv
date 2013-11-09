#include <iostream>
#include <vector>

#include <KISpMV.h>

typedef KISpMV::Matrix<double> DMat;
typedef KISpMV::Vector<double> DVec;

int main(int argc, char *argv[]) {

    const int m = 40, n = 40, nnz = 20;
    std::vector<int> rowPtrs(m+1), colInds(nnz);

    std::vector<double> vals(nnz);
    DMat M = DMat::CreateFromCSR(m, n, rowPtrs, colInds, vals);
    DVec x(n), y(m);
    y = M * x;
    double *data = &y[0];

    return 0;
}
