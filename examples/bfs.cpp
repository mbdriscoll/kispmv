#include <iostream>
#include <climits>
#include <vector>

#include <KISpMV.h>

struct Edge {
};

struct Node {
    int bfs_id;

    Node()
      : bfs_id(INT_MAX-100000)
    {  }

    Node(int id)
      : bfs_id(id)
    {  }

    inline void pluseq(const Node o) {
        bfs_id = std::min(bfs_id, o.bfs_id);
    }

    inline Node times(const Node& o, const Edge& e) {
        return Node(bfs_id+1);
    }
};

int main(int argc, char *argv[]) {

    const int m = 40, n = 40, nnz = 20;
    std::vector<int> rowInds(nnz), colInds(nnz);
    std::vector<Edge> edges(nnz);

    std::vector<Node> nodes(m);
    nodes[0].bfs_id = 0;

    KISpMV::Matrix<Edge,Node> *M =
        KISpMV::Matrix<Edge,Node>::CreateFromCOO(m, n, rowInds, colInds, edges);

    for (int i = 0; i < /*span*/10; i++)
        nodes = M * nodes;

    return 0;
}
