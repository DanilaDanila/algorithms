#include <bits/stdc++.h>

class Graph
{
public:
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

	Graph invert()
	{
		Graph result(edges.size());

		for(int from=0; from<edges.size(); ++from)
			for(int to=0; to<edges[from].size(); ++to)
				result.addEdge(edges[from][to], from);

		/*
		for(int to=0; to<edges.size(); ++to)
			for(int from=0; from<edges.size(); ++from)
				if(std::find(edges[from].begin(), edges[from].end(), to) != edges[from].end())
					result.addEdge(to, from);
		*/

		return result;
	}

	int countNodesWithoutOutput()
	{
		int result = 0;
		for(auto x : edges)
			result += x.size() == 0;
		return result;
	}

	int countNodesWithoutInput()
	{
		return this->invert().countNodesWithoutOutput();
	}

    ~Graph() {}
};

void __DFS(const Graph &g, const int &node, std::vector<int> &out, bool *visited=nullptr)
{
	bool need_to_free = visited == nullptr;
	if(need_to_free)
	{
		visited = new bool[g.size()];
		for(int i=0; i<g.size(); ++i)
			visited[i] = false;
	}

	visited[node] = true;

	auto next_nodes = g.getNeighbours(node);
	for(int x : next_nodes)
		if(!visited[x])
			__DFS(g, x, out, visited);

	out.push_back(node);

	if(need_to_free)
		delete [] visited;
}

std::vector<int> getOutOrder(const Graph &g)
{
	std::vector<int> out;

	bool *visited = new bool[g.size()];
	for(int i=0; i<g.size(); ++i)
		visited[i] = false;

	for(int i=0; i<g.size(); ++i)
		if(visited[i])
			continue;
		else
			__DFS(g, i, out, visited);

	delete [] visited;

	return out;
}

std::vector<std::vector<int>> splitGraph(const Graph &g, const std::vector<int> &order)
{
	bool *visited = new bool[g.size()];
	for(int i=0; i<g.size(); ++i)
		visited[i] = false;

	std::vector<std::vector<int>> result;

	for(auto x : order)
		if(!visited[x])
		{
			std::vector<int> new_part;
			__DFS(g, x, new_part, visited);
			std::sort(new_part.begin(), new_part.end());
	
			result.push_back(new_part);
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
		--from, --to;
		graph.addEdge(from, to);
	}

	auto order = getOutOrder(graph.invert());
	std::reverse(order.begin(), order.end());

	auto splited = splitGraph(graph, order);

	Graph splited_graph(splited.size());
	for(int i=0; i<splited.size(); ++i)
	{
		std::vector<int> fn;
		__DFS(graph, splited[i][0], fn);
		std::set<int> set(fn.begin(), fn.end());

		for(int j=0; j<splited.size(); ++j)
			if(i != j &&
			   set.find(splited[j][0]) != set.end())
				splited_graph.addEdge(i, j);
	}

	if(splited_graph.size() > 1)
		std::cout<<std::max(splited_graph.countNodesWithoutOutput(),
					    	splited_graph.countNodesWithoutInput())<<"\n";
	else
		std::cout<<0<<"\n";

	return 0;
}
