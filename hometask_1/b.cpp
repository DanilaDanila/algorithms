#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

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

bool __searchLoopsFrom(const Graph &graph, const int &node, bool *visited = nullptr)
{
	bool ret_value = false;

	bool need_to_free = false;
	if(visited == nullptr)
	{
		visited = new bool[graph.size()];
		for(int i=0; i<graph.size(); ++i)
			visited[i] = false;
		need_to_free = true;
	}

	visited[node] = true;

	auto next_nodes = graph.getNeighbours(node);
	for(int i=0; i<next_nodes.size() && !ret_value; ++i)
		if(visited[next_nodes[i]])
		{
			ret_value = true;
			break;
		}
		else
			ret_value = __searchLoopsFrom(graph, next_nodes[i], visited);

	visited[node] = false;

	if(need_to_free)
		delete [] visited;

	return ret_value;;
}

bool isLoops(const Graph &g)
{
	for(int i=0; i<g.size(); ++i)
		if (__searchLoopsFrom(g, i))
			return true;
	return false;
}

void concat(std::vector<int> &v0, const std::vector<int> &v1)
{
	for(auto x : v1)
		v0.push_back(x);
}

std::vector<int> __subtreeTopSort(const Graph &g, const int node, bool *visited)
{
	visited[node] = true;
	std::vector<int> result;
	auto next_nodes = g.getNeighbours(node);
	std::sort(next_nodes.begin(), next_nodes.end());
	for(int a : next_nodes)
		if(!visited[a])
			concat(result, __subtreeTopSort(g, a, visited));
	
	result.push_back(node);

	return result;
}

std::vector<int> topSort(const Graph &g)
{
	bool *nodes = new bool[g.size()];
	for(int i=0; i<g.size(); ++i)
		nodes[i] = false;

	for(int i=0; i<g.size(); ++i)
	{
		auto edges = g.getNeighbours(i);
		for(int x : edges)
			nodes[x] = true;
	}

	bool *visited = new bool[g.size()];
	for(int i=0; i<g.size(); ++i)
		visited[i] = false;

	std::vector<int> result;
	for(int i=0; i<g.size(); ++i)
		if(!nodes[i])
		{
			visited[i] = true;
			result.push_back(i);
		}

	delete [] nodes;

	int start_nodes_count = result.size();
	for(int i=0; i < start_nodes_count; ++i)
	{
		auto subtree = __subtreeTopSort(g, result[i], visited);
		std::reverse(subtree.begin(), subtree.end());
		subtree.erase(subtree.begin());

		concat(result, subtree);
	}

	delete [] visited;

	return result;
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
		std::cout<<"YES\n";
		auto a=topSort(graph);
		for(auto it = a.begin(); it != a.end(); ++it)
			std::cout<<*it<<" ";
		std::cout<<"\n";
	}

	return 0;
}
