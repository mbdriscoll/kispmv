#include <cstdio>
#include <cmath>
#include <cfloat>
#include <KISpMV.h>

struct Double {
    double val;

    Double()
      : val(0)
    {  }

    Double(double v)
      : val(v)
    {  }

    Double operator-(const Double& o) {
        return this->val - o.val;
    }

    operator double() {
        return val;
    }

    inline void pluseq(const Double& o) {
        val += o.val;
    }

    inline Double times(const Double& o, double& d) {
        return Double(val*d);
    }
};

int main(int argc, char *argv[]) {

    int nTrials = 10, seed = 14;
    int m = 10, n = 5, nnz = 10;
    std::vector<int> rowInds(nnz),
                     colInds(nnz);
    std::vector<double> vals(nnz);
    std::vector<Double> x(n, 1.0);

    srand48(seed);
    for (int i = 0; i < nnz; i++) {
        rowInds[i] = rand() % m;
        colInds[i] = rand() % n;
        vals[i] = drand48();
    }

    //KISpMV::Matrix<double,double> *M = KISpMV::Matrix<double,double>::CreateFromCOO(m, n, rowInds, colInds, vals);
    //std::vector<double> y = M * x;

    KISpMV::Matrix<double,Double> *M0 = KISpMV::CpuCsrMatrix<double,Double>::CreateFromCOO(m, n, rowInds, colInds, vals);
    KISpMV::Matrix<double,Double> *M1 = KISpMV::CpuCooMatrix<double,Double>::CreateFromCOO(m, n, rowInds, colInds, vals);

    std::vector<Double> y0 = M0 * x;
    std::vector<Double> y1 = M1 * x;

    int nErrors = 0;
    for (int i = 0; i < y0.size(); i++)
        if (fabs(y0[i] - y1[i]) > DBL_EPSILON)
            nErrors += 1;

    if (nErrors > 0)
        fprintf(stderr, "Error: found mistakes in %d/%d positions.\n", nErrors, (int) y0.size());
    else
        fprintf(stderr, "Success!\n");

    return 0;
}
