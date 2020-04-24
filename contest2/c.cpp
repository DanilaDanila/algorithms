#include <iostream>
#include <vector>
#include <queue>

using std::cin, std::cout, std::vector, std::pair, std::priority_queue;

class Graph
{
private:
	vector<vector<pair<int,int>>> edges;
public:
	Graph() {}

	Graph(int nodes_count):
		edges(nodes_count)
	{}

	~Graph() {}

	void addEdge(int from, int to, int w)
	{
		edges[from].push_back({to, w});
		edges[to].push_back({from, w});
	}

	const vector<pair<int,int>> getNeighbours(int node) const
	{ return edges[node]; }

	int size() const { return edges.size(); }
};

/*
 *	Находим вероятность быть НЕИЗБИТЫМ
 */
double getAntiProbability(const Graph &g, int start, int stop)
{
	bool *visited = new bool[g.size()];
	double *anti_prob_to = new double[g.size()];
	for(int i=0; i<g.size(); ++i)
	{
		visited[i] = false;
		anti_prob_to[i] = 0.0;
	}
	anti_prob_to[start] = 1.0;	

	priority_queue<pair<double, int>> next_nodes;
	next_nodes.push({1, start});

	while(!next_nodes.empty())
	{
		int cur_node = next_nodes.top().second;
		next_nodes.pop();

		if(visited[cur_node]) continue;
		visited[cur_node] = true;

		auto neighbours = g.getNeighbours(cur_node);
		for(auto x: neighbours)
		{
			const int &node = x.first;
			const int &prob_mul_100 = x.second;

			// Вероятность не быть избитым при телепорте
			double anti_prob_step = (100 - prob_mul_100) / 100.0;

			if(anti_prob_to[cur_node] * anti_prob_step > anti_prob_to[node])
			{
				anti_prob_to[node] = anti_prob_to[cur_node] * anti_prob_step;
				next_nodes.push({anti_prob_to[node], node});
			}
		}
	}

	double result = anti_prob_to[stop];

	delete [] visited;
	delete [] anti_prob_to;

	return result;
}

int main()
{
	int n, m, start, stop;
	cin >> n >> m >> start >> stop;
	--start; --stop;

	Graph g(n);
	for(int i=0; i<m; ++i)
	{
		int from, to, weight;
		cin >> from >> to >> weight;
		--from; --to;
		g.addEdge(from, to, weight);
	}

	double anti_prob = getAntiProbability(g, start, stop);
	cout << 1.0 - anti_prob << '\n';

	return 0;
}
