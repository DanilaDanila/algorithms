#include <bits/stdc++.h>

class Graph
{
private:
	std::vector<std::vector<int>> edges;
public:
	Graph() {}

	Graph(const int &nodes_count): edges(nodes_count)
	{}

	void addEdge(const int &from, const int &to)
	{
		edges[from].push_back(to);
	}

	const int size() const
	{
		return edges.size();
	}

	const std::vector<int> &getNeighbours(const int &node) const
	{
		return edges[node];
	}

	~Graph() {}
};

bool searchLoopsFrom(const Graph &g, const int &node, bool* const &in_stack, bool* const &visited)
{
	visited[node] = true;
	in_stack[node] = true;

	bool result = false;

	auto next_nodes = g.getNeighbours(node);
	for(int next : next_nodes)
	{
		if(in_stack[next])
			result = true;
		else if (!visited[next])
			result = searchLoopsFrom(g, next, in_stack, visited);
	
		if(result)
			break;
	}

	in_stack[node] = false;

	return result;
}

bool isLoops(const Graph &g)
{
	bool *visited = new bool[g.size()]();
	bool *in_stack = new bool[g.size()]();

	bool result = false;
	for(int i=0; !result && i<g.size(); ++i)
		if(!visited[i])
			result = searchLoopsFrom(g, i, in_stack, visited);

	delete [] visited;
	delete [] in_stack;
	return result;
}

void DFS(const Graph &g, const int &node, bool* const &visited, std::vector<int> &result)
{
	visited[node] = true;
	auto next_nodes = g.getNeighbours(node);
	for(int next : next_nodes)
		if(!visited[next])
			DFS(g, next, visited, result);
	result.push_back(node);
}

std::vector<int> topSort(const Graph &g)
{
	std::vector<int> dfs_return;
	bool *visited = new bool[g.size()]();

	for(int i=0; i<g.size(); ++i)
		if(!visited[i])
			DFS(g, i, visited, dfs_return);
	
	delete [] visited;

	return dfs_return;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, std::vector<T> vec)
{
	for(auto it = vec.rbegin(); it != vec.rend(); ++it)
		out<<*it<<" ";
	return out;
}

int main()
{
	int nodes_count, edges_count;
	std::cin>>nodes_count>>edges_count;

	Graph graph(nodes_count);

	for(int i=0; i<edges_count; ++i)
	{
		int from, to;
		std::cin>>from>>to;
		graph.addEdge(from, to);
	}

	if(isLoops(graph))
		std::cout<<"NO\n";
	else
	{
		auto sorted = topSort(graph);
		std::cout<<"YES\n"<<sorted<<"\n";
	}

	return 0;
}
