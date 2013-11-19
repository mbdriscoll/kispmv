#include <iostream>
#include <vector>

namespace KISpMV {

// --------------------------------------------------------------------------
// forward declaration

template <typename elt_t> class Matrix;
template <typename elt_t> class Vector;

template <typename elt_t> class CpuCsrMatrixNode;
template <typename elt_t> class CpuCooMatrixNode;
template <typename elt_t> class HybMatrixNode;
template <typename elt_t> class CpuDenseVectorNode;
template <typename elt_t> class CpuSparseVectorNode;

// --------------------------------------------------------------------------
// main matrix classes

class ASTNode {
  public:

    virtual void eval() {
    }

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
            case 2:
                return HybMatrixNode<elt_t>::CreateFromCSR(m, n, rowPtrs, colInds, vals);
                break;
            case 1:
                return CpuCooMatrixNode<elt_t>::CreateFromCSR(m, n, rowPtrs, colInds, vals);
                break;
            default:
            case 0:
                return CpuCsrMatrixNode<elt_t>::CreateFromCSR(m, n, rowPtrs, colInds, vals);
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

    static Vector<elt_t> Create(std::vector<elt_t>& v)
    {
        return Vector( new CpuDenseVectorNode<elt_t>(v) );
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

public:

    CpuCsrMatrixNode(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
      : super(m,n), rowPtrs(rowPtrs), colInds(colInds), vals(vals)
    {  }

    static Matrix<elt_t> CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
    {
        ASTNode* mn = new CpuCsrMatrixNode<elt_t>(m, n, rowPtrs, colInds, vals);
        return Matrix<elt_t>(mn);
    }

    virtual void dump() {
        std::cout << "(cpu-csr-matrix m="
            << super::m << " n=" << super::n
            << " nnz=" << nnz() << ")";
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

public:

    CpuCooMatrixNode(int m, int n,
        std::vector<int> &rowInds,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
      : super(m,n), rowInds(rowInds), colInds(colInds), vals(vals)
    {  }

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
class HybMatrixNode : public MatrixNode<elt_t> {
    typedef MatrixNode<elt_t> super;

    ASTNode *part_one,
            *part_two;

    HybMatrixNode(int m, int n,
      ASTNode *p1, ASTNode *p2)
      : super(m,n), part_one(p1), part_two(p2)
    {  }

public:
    static Matrix<elt_t> CreateFromCSR(int m, int n,
        std::vector<int> &rowPtrs,
        std::vector<int> &colInds,
        std::vector<elt_t> &vals)
    {
        // FIXME convert rowPtrs to rowInds
        Matrix<elt_t> p1 = Matrix<elt_t>::CreateFromCSR(m, n, rowPtrs, colInds, vals);
        Matrix<elt_t> p2 = Matrix<elt_t>::CreateFromCSR(m, n, rowPtrs, colInds, vals);
        ASTNode* hyb = new HybMatrixNode<elt_t>(m, n, p1.node, p2.node);
        return Matrix<elt_t>(hyb);
    }

    virtual void dump() {
        std::cout << "(hyb-matrix (";
        part_one->dump();
        std::cout << ") (";
        part_two->dump();
        std::cout << ")";
    }

    virtual int nnz() {
        return 1337;
    }
};

template <typename elt_t>
class CpuDenseVectorNode :
    public VectorNode<elt_t>,
    public std::vector<elt_t> {

public:
    CpuDenseVectorNode() {
    }

    CpuDenseVectorNode(std::vector<elt_t>& v) :
        std::vector<elt_t>(v)
    {  }

    virtual void dump() {
        std::cout << "(cpu-dense-vector nnz=" << nnz() << ")";
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
