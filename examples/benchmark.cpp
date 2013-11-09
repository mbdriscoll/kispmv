#include <iostream>
#include <vector>

#include <KISpMV.h>

class Particle    {};
class Interaction {};

class Node {};
class Edge {};

typedef KISpMV::Matrix<float>       SMat;
typedef KISpMV::Matrix<double>      DMat;
typedef KISpMV::Matrix<Interaction> IMat;

int main(int argc, char *argv[]) {

    const int m = 40, n = 40, nnz = 20;
    std::vector<int> rowPtrs(m+1), colInds(nnz);

    // doubles
    std::vector<double> vals(nnz);
    DMat *M = DMat::CreateFromCSR(m, n, rowPtrs, colInds, vals);

#if 0
    y = M * x;
    double *data = &y[0];

    // particles
    std::vector<Interaction> interactions(nnz);
    PVec xp(n), yp(m);
    IMat Mp = IMat::CreateFromCSR(m, n, &rowPtrs[0], &colInds[0], &interactions[0]);
    yp = Mp * xp;
    Particle *pdata = &yp[0];
#endif

    return 0;
}
