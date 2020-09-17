#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::pair;
using std::make_pair;
using std::sort;

class Graph {
  private:
    vector<pair<int, pair<int, int>>> graph;
    int vertex_count;

  public:
    Graph() {}

    Graph(int vertex_count):
        vertex_count(vertex_count)
    {}

    ~Graph() = default;

    void addEdge(int from, int to, int w) {
        graph.push_back(make_pair(w, make_pair(from, to)));
    }

    int edgesCount() const { return graph.size(); }

    int vertexCount() const { return vertex_count; }

    int getFrom(int i) const { return graph[i].second.first; }

    int getTo(int i) const { return graph[i].second.second; }

    int getW(int i) const { return graph[i].first; }

    pair<int, pair<int, int>> getEdge(int i) const { return graph[i]; }

    vector<pair<int, pair<int, int>>> getEdges() const { return graph; }
};

int main() {
    int n, m;
    cin >> n >> m;

    Graph graph(n);

    for (int i = 0; i < m; ++i) {
        int from, to, w;
        cin >> from >> to >> w;
        graph.addEdge(from-1, to-1, w);
    }

    Graph result;

    auto edges = graph.getEdges();
    sort(edges.begin(), edges.end());

    vector<int> color(n);
    for (int i = 0; i < n; ++i)
        color[i] = i;

    int from, to, w, old_color, new_color;
    for (int i = 0; i < m; ++i) {
        from = edges[i].second.second;
        to = edges[i].second.first;
        w = edges[i].first;

        if (color[from] != color[to]) {
            new_color = color[from];
            old_color = color[to];
            result.addEdge(from, to, w);

            for (int i = 0; i < n; ++i)
                if (color[i] == old_color)
                    color[i] = new_color;
        }
    }

    int sum = 0;
    for (int i = 0; i < result.edgesCount(); ++i)
        sum+=result.getW(i);
    cout << sum << '\n';

    return 0;
}
