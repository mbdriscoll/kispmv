#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>

#include <KISpMV.h>

typedef KISpMV::Matrix<double> DMat;
typedef KISpMV::Vector<double> DVec;

int main(int argc, char *argv[]) {

    static const int rowPtrsV[] = {0,  3,  5,  8, 11, 13,                      };
    static const int colIndsV[] = {0,  1,  3,  0,  1,  2, 3, 4,  0, 2, 3, 1,  4};
    static const double valsV[] = {1, -1, -3, -2,  5,  4, 6, 4, -4, 2, 7, 8, -5};
    static const double xV[] = {1, 2, 3, 4, 5};
    static const double yV[] = {0, 0, 0, 0, 0};

    std::vector<int> rowPtrs(rowPtrsV, rowPtrsV+sizeof(rowPtrsV) / sizeof(int));
    std::vector<int> colInds(colIndsV, colIndsV+sizeof(colIndsV) / sizeof(int));
    std::vector<double> vals(valsV, valsV+sizeof(valsV) / sizeof(double));
    std::vector<double> xv(xV, xV+sizeof(xV)/sizeof(double));
    std::vector<double> yv(yV, yV+sizeof(yV)/sizeof(double));
    int m = rowPtrs.size()-1,
        n = 5;

    DMat M = DMat::CreateFromCSR(m, n, rowPtrs, colInds, vals);
    DVec x = DVec::Create(xv),
         y = DVec::Create(yv);
    y = M * x;

    y.dump();

    double actual_answer = y[0];

    double expected_answer = 13.0;
    if (std::abs(actual_answer - expected_answer) > DBL_EPSILON)
        fprintf(stderr, "Error: expected %g, got %g\n", expected_answer, actual_answer);
    else
        fprintf(stderr, "Success: got element %g\n", actual_answer);

    return 0;
}
