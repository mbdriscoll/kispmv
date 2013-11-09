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
    DVec x = DVec::Create(n);

    for (int i = 0; i < 3; i++)
        x = M * x;

    double *data = &x[0];

    return 0;
}
