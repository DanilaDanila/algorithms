#include <iostream>
#include <vector>
#include <queue>

typedef long long			ill;
typedef unsigned long long	ull;

using std::vector, std::pair, std::cin, std::cout, std::priority_queue;

class Graph
{
private:
	// first - node_num, second - weight
	vector<vector<pair<ill,ill>>> edges;	

public:
	Graph() {}

	Graph(ill nodes_count):
		edges(nodes_count)
	{}

	~Graph() {}

	void addEdge(ill from, ill to, ill weight)
	{
		edges[from].push_back(pair<ill,ill>(to, weight));
	}

	ill size() const { return edges.size(); }

	const vector<pair<ill, ill>>& getNeighbours(ill node) const { return edges[node]; }
};

Graph createGraph(ill a, ill b, ill M)
{
	Graph g(M);

	for(ill i=0; i<M; ++i)
	{
		g.addEdge(i, (i+1)%M, a);
		g.addEdge(i, (i*i+1)%M, b);
	}

	return g;
}

/*
 * Поиск минимального поти из from в to,
 * используя алгоритм Дейкстры
 */
ill getShortestWay(const Graph &g, ill from, ill to)
{
	ull *distance_to = new ull[g.size()];
	bool *visited = new bool[g.size()];
	for(ill i=0; i<g.size(); ++i)
	{
		distance_to[i] = -1;
		visited[i] = false;
	}
	distance_to[from] = 0;	

	priority_queue<pair<ill, ill>> next_nodes;
	next_nodes.push({0, from});

	while(!next_nodes.empty())
	{
		ill cur_node = next_nodes.top().second;
		next_nodes.pop();

		if (visited[cur_node]) continue;
		visited[cur_node] = true;

		auto neighbours = g.getNeighbours(cur_node);
		for(auto x: neighbours)
		{
			const ill &node = x.first;
			const ill &w = x.second;

			if(distance_to[cur_node] + w < distance_to[node])
			{
				distance_to[node] = distance_to[cur_node]+w;
				next_nodes.push({-distance_to[node], node});
				// Знак минус - это костыль
				// Из очереди достаются элементы с МАКСИМАЛЬНЫМ приоритетом
				// А так получится наоборот
				// Т.е. чем меньше расстояние, тем выше приоритет
			}
		}
	}

	ill result = distance_to[to];

	delete [] visited;
	delete [] distance_to;

	return result;
}

int main()
{
	ill a, b, M, start, stop;
	cin>>a>>b>>M>>start>>stop;

	if (start == stop)
		cout << 0 << "\n";
	else
	{
		Graph g = createGraph(a, b, M);

		cout << getShortestWay(g, start, stop) << "\n";
	}

	return 0;
}
