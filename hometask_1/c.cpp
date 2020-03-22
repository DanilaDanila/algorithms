#include <algorithm>
#include <vector>
#include <iostream>

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

	bool isEdge(const int &from, const int &to) const
	{
		return std::find(edges[from].begin(), edges[from].end(), to) != edges[from].end();
	}

    const int size() const
    {
        return edges.size();
    }

    const std::vector<int> &getNeighbours(const int &node) const
    {
        return edges[node];
    }

	Graph inverted() const
	{
		Graph result(edges.size());

		for(int from=0; from<edges.size(); ++from)
   			for(int to=0; to<edges[from].size(); ++to)
    			result.addEdge(edges[from][to], from);

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
  		bool *nodes = new bool[edges.size()];
 		for(int i=0; i<edges.size(); ++i)
   			nodes[i] = false;

  		for(auto e : edges)
   			for(auto x : e)
    			nodes[x] = true;

  		int count = 0;
  		for(int i=0; i<edges.size(); ++i)
   			count += !nodes[i];

  		delete [] nodes;

  		return count;
 	}

    ~Graph() {} 
};

<<<<<<< HEAD
void DFS(const Graph &g, const int &node, std::vector<int> &t_out, bool *visited)
=======
void DFS(const Graph &g, const int &node, std::vector<int> &t_out, bool* const visited)
>>>>>>> aebb3f867c664cccde28abceaad5460d7630c495
{
	visited[node] = true;
	for(auto x : g.getNeighbours(node))
		if(!visited[x])
			DFS(g, x, t_out, visited);
	t_out.push_back(node);
}

std::vector<int> getTOut(const Graph &g)
{
	std::vector<int> result;

	bool *visited = new bool[g.size()]();

	for(int i=0; i<g.size(); ++i)
		if(!visited[i])
			DFS(g, i, result, visited);

	delete [] visited;

	return result;
}

std::vector<int> getCouplingComponents(const Graph &g)
{
	auto order = getTOut(g.inverted());

	bool *visited = new bool[g.size()]();

	std::vector<int> result(g.size());

	int i=0;
	for(auto it = order.rbegin(); it != order.rend(); ++it)
		if(!visited[*it])
		{
			std::vector<int> coupling;
			DFS(g, *it, coupling, visited);

			for(int x : coupling)
				result[x] = i;
			++i;
		}

	delete [] visited;

	return result;
}

int main()
{
	int nodes_count, edges_count;
	std::cin >> nodes_count >> edges_count;
	Graph graph(nodes_count);

	for(int i=0; i<edges_count; ++i)
	{
		int from, to;
		std::cin >> from >> to;
		--from, --to;
		graph.addEdge(from, to);
	}

	auto couplings = getCouplingComponents(graph);
	int town_size=*std::max_element(couplings.begin(), couplings.end()) + 1;
	Graph town(town_size);

	for(int i=0; i<couplings.size(); ++i)
		for(auto c : graph.getNeighbours(i))
			if(couplings[c] != couplings[i] && 
			   !town.isEdge(couplings[i], couplings[c]))
						town.addEdge(couplings[i], couplings[c]);

	if(town.size() > 1)
<<<<<<< HEAD
		std::cout<<std::max(town.countNodesWithoutOutput(),
							town.countNodesWithoutInput())<<"\n";
	else
		std::cout<<"0\n";
=======
		std::cout << std::max(town.countNodesWithoutOutput(),
							  town.countNodesWithoutInput()) <<"\n";
	else
		std::cout << "0\n";
>>>>>>> aebb3f867c664cccde28abceaad5460d7630c495

	return 0;
}
