#include <cstdio>
#include <cstdlib>

#include <KISpMV.h>

struct Point {
    int x, y, z;

    Point()
      : x(0), y(0), z(0)
    {  }

    Point(float val)
      : x(val), y(val), z(val)
    {  }

    Point(float x, float y, float z)
      : x(x), y(y), z(z)
    {  }

    bool operator==(const Point& o) const {
        return x == o.x &&
               y == o.y &&
               z == o.z;
    }

    inline void pluseq(const Point& o) {
        x += o.x;
        y += o.y;
        z += o.z;
    }

    Point times(const Point& o, const double& val) const {
        return Point(x*val, y*val, z*val);
    }
};

int main(int argc, char *argv[]) {

    int nTrials = 100,
        m = 100,
        n = 100,
        nnz = 1000,
        seed = 13;

    std::vector<int> rowInds(nnz),
                     colInds(nnz);
    std::vector<double> vals(nnz);
    std::vector<Point> x(n);

    srand(seed);
    for (int i = 0; i < nnz; i++) {
        rowInds[i] = rand() % m;
        colInds[i] = rand() % n;
        vals[i] = drand48() / RAND_MAX + 1.0;
    }

    KISpMV::Matrix<double,Point> *M =
        KISpMV::Matrix<double,Point>::CreateFromCOO(m, n, rowInds, colInds, vals);

    std::vector<Point> y_act;
    for (int t = 0; t < nTrials; t++)
        y_act = M * x;

    return 0;
}
