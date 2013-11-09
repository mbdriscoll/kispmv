#include <iostream>
#include <climits>
#include <vector>

#include <KISpMV.h>

class Particle {
    float x,y, vx,vy, ax, ay;
  public:
    Particle& operator+=(const Particle o) {
        this->x += o.x;
        this->y += o.y;
        return *this;
    }
};

class Contact {
  public:
    Particle operator*(Particle& p) {
        return p;
    }
};

class Node {
    int bfs_id;
  public:

    Node() : bfs_id(INT_MAX) { }

    Node operator+=(const Node o) {
        bfs_id = std::min(bfs_id, o.bfs_id+1);
        return *this;
    }
};

class Edge {
  public:
    Node operator*(Node& o) {
        return o;
    }
};

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


typedef KISpMV::Matrix<double>      DMat;
typedef KISpMV::Vector<double>      DVec;

typedef KISpMV::Vector<Particle>    PVec;
typedef KISpMV::Matrix<Contact>     IMat;

typedef KISpMV::Vector<Node>        NVec;
typedef KISpMV::Matrix<Edge>        EMat;

typedef KISpMV::Vector<Point>       VVec;
typedef KISpMV::Matrix<float>       FMat;

// database joins
// vertex coloring in trees
// automatic s-step methods
// auto beamer-ize bfs?
// ca spmv via ca nbody?
// KI Blas?
// implicit matrices - stencil?
// higher order systems

int main(int argc, char *argv[]) {

    const int m = 40, n = 40, nnz = 20;
    std::vector<int> rowPtrs(m+1), colInds(nnz);

    // regular spmv with doubles
    std::vector<double> vals(nnz);
    DMat M = DMat::CreateFromCSR(m, n, rowPtrs, colInds, vals);
    DVec x(n), y(m);
    y = M * x;
    double *data = &y[0];

    // particles
    std::vector<Contact> contacts(nnz);
    IMat Mi = IMat::CreateFromCSR(m, n, rowPtrs, colInds, contacts);
    PVec xp(n), yp(m);
    yp = Mi * xp;
    Particle *pdata = &yp[0];

    // bfs
    std::vector<Edge> edges(nnz);
    EMat Me = EMat::CreateFromCSR(m, n, rowPtrs, colInds, edges);
    NVec xn(n), yn(m);
    yn = Me * xn;
    Node *ndata = &yn[0];

    // subd surface evaluation / csrmm
    std::vector<float> coeffs(nnz);
    FMat Mf = FMat::CreateFromCSR(m, n, rowPtrs, colInds, coeffs);
    VVec xv(n), yv(m);
    yv = Mf * xv;
    Point *vdata = &yv[0];

    return 0;
}
