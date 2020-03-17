#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

class Graph
{                                                           //                             vector<unsorted_set<int>>   vector<pair<int, int>>
public:                                                     // matrix{mem(V^2)}     list{mem(E+V)}  set{mem(E+V)}       edges
    Graph(int vertex_count);                                //  O(V^2)                  O(V)            O(V)
    void addVertex();                                       //  O(V)         !          O(1)            O(1)
    void addEdge(int from, int to);                         //  O(1)                    O(1)            O(1)
    bool hasEdge(int from, int to);                         //  O(1)         !          O(V)            O(1)
    const vector<int>& getNextVertexes(int from) const;     //  O(V)         !          O(1)            O(1)
    int vertexCount() const;                                //  O(1)                    O(1)            O(1)

private:
};

void BFS(const Graph &graph, int from, void (visit)(int))
{
    std::queue<int> qu;
    std::vector<bool> color(graph.vertexCount(), false);

    qu.push(from);

    while(!qu.empty())
    {
        int vertex = qu.front();
        qu.pop();
        color[vertex] = true;

        const vector<int>& vv = graph.getNextVertexes(vertex);
        for(int v : vv)
            if(!color[v])
            {
                color[vertex] = true;
                qu.push(v);
            }

        visit(vertex);
    }
}

// not true DFS
void DFS(const Graph &graph, int from, void (visit)(int))
{
    std::stack<int> st;
    st.push(from);

    std::vector<bool> visited(graph.vertexCount(), false);
    visited[from] = true;

    while(!st.empty())
    {
        int vertex = st.top();
        st.pop();

        const vector<int> next = graph.getNextVertexes(vertex);
        for(int v : next)
        {
            visited[v] = true;
            st.push(v);
        }

        visit(vertex);
    }
}

int main()
{
    return 0;
}