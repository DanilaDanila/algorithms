#include <iostream>
#include <vector>
#include <utility>
#include <queue>

using std::vector, std::pair, std::priority_queue, std::cin, std::cout;

class Graph
{
private:
	vector<vector<pair<int, int>>> edges;

public:
	Graph() {}

	Graph(int nodes_count):
		edges(nodes_count)
	{}

	~Graph() {}

	void addEdge(int from, int to, int weight)
	{
		edges[from].push_back({to, weight});
	}

	const vector<pair<int,int>> getNeighbours(int node) const
	{ return edges[node]; }

	int size() const { return edges.size(); }
};

int findMinCoast(const Graph &g, int limit, int start, int stop)
{
	unsigned *distance_to = new unsigned[g.size()];
	unsigned char *jumps_to = new unsigned char[g.size()];
	bool *visited = new bool[g.size()];
	for(int i=0; i<g.size(); ++i)
	{
		distance_to[i] = -1;
		jumps_to[i] = -1;
		visited[i] = false;
	}
	distance_to[start] = 0;
	jumps_to[start] = 0;

	priority_queue<pair<int, int>> next_nodes;	
	next_nodes.push({0, start});

	while(!next_nodes.empty())
	{
		int cur_node = next_nodes.top().second;
		next_nodes.pop();

		if(visited[cur_node]) continue;
		if(jumps_to[cur_node] >= limit) continue;
		visited[cur_node] = true;

		auto neighbours = g.getNeighbours(cur_node);
		for(auto x: neighbours)
		{
			const int &node = x.first;
			const int &w = x.second;
			
			if(distance_to[cur_node] + w < distance_to[node])
			{
				distance_to[node] = distance_to[cur_node] + w;
				jumps_to[node] = jumps_to[cur_node] + 1;
				next_nodes.push({-distance_to[node], node});
			}
		}
	}

	int result = distance_to[stop];

	delete [] distance_to;
	delete [] jumps_to;
	delete [] visited;

	return result;
}

int main()
{
	int n, m, k, start, stop;
	cin >> n >> m >> k >> start >> stop;
	--start; --stop;

	Graph g(n);
	for(int i=0; i<m; ++i)
	{
		int from, to, weight;
		cin >> from >> to >> weight;
		--from; --to;

		g.addEdge(from, to, weight);
	}

	cout << (int)findMinCoast(g, k, start, stop) << '\n';

	return 0;
}
