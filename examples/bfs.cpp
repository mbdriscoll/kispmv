#include <iostream>
#include <climits>
#include <vector>

#include <KISpMV.h>

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

typedef KISpMV::Vector<Node> NVec;
typedef KISpMV::Matrix<Edge> EMat;

int main(int argc, char *argv[]) {

    const int m = 40, n = 40, nnz = 20;
    std::vector<int> rowPtrs(m+1), colInds(nnz);

    std::vector<Edge> edges(nnz);
    EMat Me = EMat::CreateFromCSR(m, n, rowPtrs, colInds, edges);
    NVec xn(n), yn(m);
    yn = Me * xn;
    Node *ndata = &yn[0];

    return 0;
}
