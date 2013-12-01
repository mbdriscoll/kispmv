#include <iostream>
#include <vector>

#include <KISpMV.h>

class Contact {
};

class Particle {
    float x,y, vx,vy, ax, ay;
  public:
    Particle()
      : x(0), y(0), vx(0), vy(0), ax(0), ay(0)
    {  }

    Particle(float n)
      : x(n), y(n), vx(n), vy(n), ax(n), ay(n)
    {  }

    Particle& operator+=(const Particle& o) {
        this->x += o.x;
        this->y += o.y;
        return *this;
    }

    Particle operator*(Contact& p) {
        return *this;
    }
};

int main(int argc, char *argv[]) {

    const int m = 40, n = 40, nnz = 20;
    std::vector<int> rowInds(nnz), colInds(nnz);

    std::vector<Particle> x(n);
    std::vector<Contact> contacts(nnz);
    KISpMV::Matrix<Contact,Particle> *M =
        KISpMV::Matrix<Contact,Particle>::CreateFromCOO(m, n, rowInds, colInds, contacts);

    std::vector<Particle> y = M * x;;

    return 0;
}
