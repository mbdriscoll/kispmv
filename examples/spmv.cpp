#include <iostream>
#include <vector>

#include <KISpMV.h>

typedef KISpMV::Matrix<double> DMat;
typedef KISpMV::Vector<double> DVec;

int main(int argc, char *argv[]) {

    static const int rowPtrsV[] = {0,  3,  5,  8, 11, 13,                      };
    static const int colIndsV[] = {0,  1,  3,  0,  1,  2, 3, 4,  0, 2, 3, 1,  4};
    static const double valsV[] = {1, -1, -3, -2,  5,  4, 6, 4, -4, 2, 7, 8, -5};

    std::vector<int> rowPtrs(rowPtrsV, rowPtrsV+sizeof(rowPtrsV) / sizeof(int));
    std::vector<int> colInds(colIndsV, colIndsV+sizeof(colIndsV) / sizeof(int));
    std::vector<double> vals(valsV, valsV+sizeof(valsV) / sizeof(double));
    int m = rowPtrs.size()-1,
        n = 5;

    DMat M = DMat::CreateFromCSR(m, n, rowPtrs, colInds, vals);
    DVec x = DVec::Create(n),
         y = DVec::Create(m);
    y = M * x;
    double *data = &y[0];

    return 0;
}
