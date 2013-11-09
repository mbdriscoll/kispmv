#include <iostream>
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

typedef KISpMV::Vector<Particle> PVec;
typedef KISpMV::Matrix<Contact>  IMat;

int main(int argc, char *argv[]) {

    const int m = 40, n = 40, nnz = 20;
    std::vector<int> rowPtrs(m+1), colInds(nnz);

    std::vector<Contact> contacts(nnz);
    IMat Mi = IMat::CreateFromCSR(m, n, rowPtrs, colInds, contacts);
    PVec xp = PVec::Create(n),
         yp = PVec::Create(m);
    yp = Mi * xp;
    Particle *pdata = &yp[0];

    return 0;
}
