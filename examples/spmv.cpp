#include <cstdio>
#include <KISpMV.h>

int main(int argc, char *argv[]) {

    int nTrials = 10, seed = 13;
    int m = 10, n = 5, nnz = 10;
    std::vector<int> rowInds(nnz),
                     colInds(nnz);
    std::vector<double> valsd(nnz);
    std::vector<float>  valsf(nnz);
    std::vector<double> xd(n, 1.0);
    std::vector<float>  xf(n, 1.0);

    srand48(seed);
    for (int i = 0; i < nnz; i++) {
        rowInds[i] = rand() % m;
        colInds[i] = rand() % n;
        valsf[i] = valsd[i] = drand48();
    }

    KISpMV::CpuCsrMatrix<double> M_cpucsrd = KISpMV::CpuCsrMatrix<double>::CreateFromCOO(m, n, rowInds, colInds, valsd);
    KISpMV::CpuCsrMatrix<float>  M_cpucsrf = KISpMV::CpuCsrMatrix<float> ::CreateFromCOO(m, n, rowInds, colInds, valsf);
    KISpMV::CpuCooMatrix<double> M_cpucood = KISpMV::CpuCooMatrix<double>::CreateFromCOO(m, n, rowInds, colInds, valsd);
    KISpMV::GpuCooMatrix<double> M_gpucood = KISpMV::GpuCooMatrix<double>::CreateFromCOO(m, n, rowInds, colInds, valsd);

    std::vector<double> y_cpucsrd = M_cpucsrd * xd;
    std::vector<float>  y_cpucsrf = M_cpucsrf * xf;
    std::vector<double> y_cpucood = M_cpucood * xd;
    std::vector<double> y_gpucood = M_gpucood * xd;

    return 0;
}
