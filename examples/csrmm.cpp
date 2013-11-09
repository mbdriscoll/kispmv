#include <iostream>
#include <vector>

#include <KISpMV.h>

class Point {
    float x, y, z;

  public:
    Point()
      : x(0), y(0), z(0)
    { }

    Point operator+=(const Point o) {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }

  friend Point operator*(float,Point);
};

Point operator*(float f, Point rhs) {
    Point p;
    p.x = f * rhs.x;
    p.y = f * rhs.y;
    p.z = f * rhs.z;
    return p;
}


typedef KISpMV::Vector<Point> VVec;
typedef KISpMV::Matrix<float> FMat;

int main(int argc, char *argv[]) {

    const int m = 40, n = 40, nnz = 20;
    std::vector<int> rowPtrs(m+1), colInds(nnz);

    std::vector<float> coeffs(nnz);
    FMat Mf = FMat::CreateFromCSR(m, n, rowPtrs, colInds, coeffs);
    VVec xv(n), yv(m);
    yv = Mf * xv;
    Point *vdata = &yv[0];

    return 0;
}
