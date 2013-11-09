#include <iostream>
#include <vector>

namespace KISpMV {

// --------------------------------------------------------------------------
// forward declaration

template <typename elt_t> class Matrix;
template <typename elt_t> class Vector;


// --------------------------------------------------------------------------
// main matrix classes

class ASTNode {
  public:
    virtual void dump() {
        std::cout << "(astnode)";
    }

    ASTNode operator*(ASTNode rhs) {
        return *this;
    }

    virtual std::ostream& operator<<(std::ostream& out) {
        return out << "foo";
    }
};

class ASTNodeRef {
  public:
    ASTNode *node;

    ASTNodeRef(ASTNode *o)
      : node(o)
    {  }
};

class MatrixNode : public ASTNode {
    int m, n;

public:
    MatrixNode(int m, int n)
      : m(m), n(n)
    {  }

    int nnz() {
        return 1337;
    }

    virtual void dump() {
        std::cout << "(matrix "
            << m << " " << n << " "
            << nnz() << ")";
    }
};

class MultNode : public ASTNode {
    ASTNode *lhs, *rhs;

  public:
    MultNode(ASTNode *lhs, ASTNode *rhs)
      : lhs(lhs), rhs(rhs)
    {  }

    virtual void dump() {
        std::cout << "(* ";
        lhs->dump();
        std::cout << " ";
        rhs->dump();
        std::cout << ")";;
    }
};

class VectorNode : public ASTNode {
    virtual void dump() {
        std::cout << "(vector 1338)";
    }
};

template <typename elt_t>
class Matrix : public ASTNodeRef {
    Matrix(ASTNode *n)
      : ASTNodeRef(n)
    { }

public:
    static Matrix<elt_t> CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrsSrc,
        std::vector<int> &colIndsSrc,
        std::vector<elt_t> &valsSrc)
    {
        ASTNode* mn = new MatrixNode(m, n);
        return Matrix(mn);
    }

    template <typename VT>
    Vector<VT> operator*(Vector<VT> v) {
        return Vector<VT>( new MultNode(node, v.node) );
    }
};

template <typename elt_t>
class Vector : public ASTNodeRef {
public:
    Vector(ASTNode *underlying)
      : ASTNodeRef(underlying)
    {  }

    static Vector<elt_t> Create(int nElems)
    {
        ASTNode* vn = new VectorNode();
        return Vector(vn);
    }

    elt_t& operator[](int i) {
        std::vector<elt_t> tmp(10);
        node->dump();
        std::cout << std::endl;
        return tmp[0];
    }
};

} /* namespace KISpMV */
