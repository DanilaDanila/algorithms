#include <iostream>
#include <utility>
#include <queue>
#include <vector>

class Graph
{
private:
	int nodes_count;
	int edges_count;

	std::vector<std::vector<int>> edges_list;

public:
	Graph() {}

	Graph(const int nodes_count, const int edges_count):
		nodes_count(nodes_count),
		edges_count(edges_count),
	   	edges_list(nodes_count)
		{}

	void addEdge(int from, int to)
	{
		edges_list[from].push_back(to);
		edges_list[to].push_back(from);
	}

	const std::vector<int> getNeighbours(int node) const
	{
		return edges_list[node];
	}

	const int getNodesCount() const
	{
		return nodes_count;
	}

	~Graph() {}
};

std::vector<int> BFS(const Graph &graph, const int node)
{
	std::vector<int> result(graph.getNodesCount(), -1);

	std::queue<std::pair<int,int>> queue;
	queue.push(std::pair<int,int>(node,0));

	result[node] = 0;

	while(!queue.empty())
	{
		auto cur = queue.front();
		queue.pop();

		int &cur_node = cur.first;
		int &cur_dist = cur.second;

		auto next_nodes = graph.getNeighbours(cur_node);

		for(int i : next_nodes)
			if(result[i] == -1)
			{
				queue.push(std::pair<int, int>(i, cur_dist+1));
				result[i] = cur_dist+1;
			}
	}

	return result;
}

int main()
{
	int nodes, edges, leon, mati, milk;
	std::cin >> nodes >> edges >> leon >> mati >> milk;
	--leon, --mati, --milk;

	Graph graph(nodes, edges);

	for(int i=0; i<edges; ++i)
	{
		int from, to;
		std::cin >> from >> to;
		--from, --to;
		graph.addEdge(from, to);
	}


	const char to_leon = 0;
	const char to_mati = 1;
	const char to_milk = 2;
	std::vector<int> distance[3];
	distance[to_leon] = BFS(graph, leon);
	distance[to_mati] = BFS(graph, mati);
	distance[to_milk] = BFS(graph, milk);

	int min_sum = distance[0][0] + distance[1][0] + distance[2][0];
	for(int i=0; i<nodes; ++i)
	{
		int new_sum = distance[0][i] + distance[1][i] + distance[2][i];

		if(new_sum < min_sum)
			min_sum = new_sum;
	}

	std::cout<<min_sum<<"\n";
	
	return 0;
}
