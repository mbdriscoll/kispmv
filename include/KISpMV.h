#include <iostream>
#include <vector>

namespace KISpMV {

// --------------------------------------------------------------------------
// forward declaration

template <typename elt_t> class Matrix;
template <typename elt_t> class Vector;

template <typename elt_t> class CpuCsrMatrixNode;
template <typename elt_t> class CpuCooMatrixNode;
template <typename elt_t> class CpuDenseVectorNode;
template <typename elt_t> class CpuSparseVectorNode;

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

template <typename elt_t>
class MatrixNode : public ASTNode {
protected:
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

template <typename elt_t>
class VectorNode : public ASTNode {
    virtual void dump() {
        std::cout << "vector";
    }
};

template <typename elt_t>
class Matrix : public ASTNodeRef {
public:
    Matrix(ASTNode *n)
      : ASTNodeRef(n)
    { }

    template <typename VT>
    Vector<VT> operator*(Vector<VT> v) {
        return Vector<VT>( new MultNode(node, v.node) );
    }

    static Matrix<elt_t> CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
    {
        int format_id = 0;
        switch (format_id) {
            default:
            case 0:
                return CpuCsrMatrixNode<elt_t>::CreateFromCSR(m, n, rowPtrs, colInds, vals);
                break;
            case 1:
                return CpuCooMatrixNode<elt_t>::CreateFromCSR(m, n, rowPtrs, colInds, vals);
                break;
        }
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
        int format_id = 0;
        switch (format_id) {
            default:
            case 0:
                return Vector( new CpuDenseVectorNode<elt_t>() );
                break;
            case 1:
                return Vector( new CpuSparseVectorNode<elt_t>() );
                break;
        }
    }

    elt_t& operator[](int i) {
        std::vector<elt_t> tmp(10);
        node->dump();
        std::cout << std::endl;
        return tmp[0];
    }
};

template <typename elt_t>
class CpuCsrMatrixNode : public MatrixNode<elt_t> {
    typedef MatrixNode<elt_t> super;

    std::vector<int> rowPtrs, colInds;
    std::vector<elt_t> vals;

    CpuCsrMatrixNode(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
      : super(m,n), rowPtrs(rowPtrs), colInds(colInds), vals(vals)
    {  }

public:
    static Matrix<elt_t> CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
    {
        ASTNode* mn = new CpuCsrMatrixNode<elt_t>(m, n, rowPtrs, colInds, vals);
        return Matrix<elt_t>(mn);
    }

    virtual void dump() {
        std::cout << "(cpu-csr-matrix " << nnz() << ")";
    }

    virtual int nnz() {
        return rowPtrs[super::m];
    }
};

template <typename elt_t>
class CpuCooMatrixNode : public MatrixNode<elt_t> {
    typedef MatrixNode<elt_t> super;

    std::vector<int> rowInds, colInds;
    std::vector<elt_t> vals;

    CpuCooMatrixNode(int m, int n,
        std::vector<int> &rowInds,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
      : super(m,n), rowInds(rowInds), colInds(colInds), vals(vals)
    {  }

public:
    static Matrix<elt_t> CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
    {
        // FIXME convert rowPtrs to rowInds
        ASTNode* mn = new CpuCooMatrixNode<elt_t>(m, n, rowPtrs, colInds, vals);
        return Matrix<elt_t>(mn);
    }

    virtual void dump() {
        std::cout << "(cpu-coo-matrix " << nnz() << ")";
    }

    virtual int nnz() {
        return vals.size();
    }
};

template <typename elt_t>
class CpuDenseVectorNode :
    public VectorNode<elt_t>,
    public std::vector<elt_t> {

    virtual void dump() {
        std::cout << "(cpu-dense-vector " << nnz() << ")";
    }

    virtual int nnz() {
        return std::vector<elt_t>::size();
    }
};

template <typename elt_t>
class CpuSparseVectorNode : public VectorNode<elt_t> {

protected:
    std::vector<elt_t> vals;
    std::vector<int> inds;

    virtual void dump() {
        std::cout << "(cpu-sparse-vector " << nnz() << ")";
    }

    virtual int nnz() {
        return vals.size();
    }
};

} /* namespace KISpMV */
