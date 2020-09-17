#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <set>

using std::cin;
using std::cout;
using std::vector;
using std::set;
using std::pair;
using std::make_pair;
using std::numeric_limits;

class Graph {
  private:
    int edges;
    vector<vector<pair<int, int>>> graph;

  public:
    Graph() {}

    Graph(int vertex_count, int edges):
        edges(edges) {
        graph.assign(vertex_count, vector<pair<int, int>>());
    }

    ~Graph() = default;

    void addEdge(int from, int to, int w) {
        graph[from].emplace_back(to, w);
        graph[to].emplace_back(from, w);
    }

    bool hasEdge(int from, int to) const {
        for (int i = 0; i < graph[from].size(); i++)
            if (graph[from][i].first == to)
                return true;
    }

    int getEdgesCount() const { return edges; }

    int size() const { return graph.size(); }

    const vector<pair<int, int>> &getNeighbours(int node) const { return graph[node]; }
};

// Prim
vector<int> createMST(const Graph &g) {
    vector<int> mst(g.size(), numeric_limits<int>::max());

    set<pair<int, int>> q;
    q.insert({0, 0});
    int to, node, w;
    mst[0] = 0;
    vector<bool> used(g.size(), false);

    while (!q.empty()) {
        node = q.begin()->second;
        q.erase(q.begin());

        for (int j = 0; j < g.getNeighbours(node).size(); ++j) {
            used[node] = true;
            to = g.getNeighbours(node)[j].first;
            w = g.getNeighbours(node)[j].second;

            if (w < mst[to] && !used[to]) {
                q.erase(pair<int, int>(mst[to], to));
                mst[to] = w;
                q.emplace(w, to);
            }
        }
    }

    return mst;
}

int main() {
    int n, m;
    cin >> n >> m;

    Graph graph(n, m);

    for (int i = 0; i < graph.getEdgesCount(); ++i) {
        int from, to, w;
        cin >> from >> to >> w;
        graph.addEdge(from-1, to-1, w);
    }

    vector<int> mst = createMST(graph);

    int sum = 0;
    for (int i = 0; i < n; ++i)
        sum += mst[i];

    cout << sum;
    return 0;
}
