#include <iostream>
#include <vector>

#include <KISpMV.h>

class Particle    {};
class Interaction {};

class Node {};
class Edge {};

typedef KISpMV::Matrix<float,float,float> SMat;
typedef KISpMV::Vector<float>             SVec;

typedef KISpMV::Matrix<double,double,double> DMat;
typedef KISpMV::Vector<double>               DVec;

typedef KISpMV::Matrix<Particle,Interaction,Particle> IMat;
typedef KISpMV::Vector<Particle>                      PVec;

int main(int argc, char *argv[]) {

    const int m = 40, n = 40, nnz = 20;
    std::vector<int> rowPtrs(m+1), colInds(nnz);

    // doubles
    std::vector<double> vals(nnz);
    DVec x(n), y(m);
    DMat M = DMat::CreateFromCSR(m, n, &rowPtrs[0], &colInds[0], &vals[0]);
    y = M * M * x;
    double *data = &y[0];

    // particles
    std::vector<Interaction> interactions(nnz);
    PVec xp(n), yp(m);
    IMat Mp = IMat::CreateFromCSR(m, n, &rowPtrs[0], &colInds[0], &interactions[0]);
    yp = Mp * xp;
    Particle *pdata = &yp[0];

    return 0;
}
