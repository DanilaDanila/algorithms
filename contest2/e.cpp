#include <iostream>
#include <ostream>
#include <vector>
#include <utility>
#include <tuple>

using std::cin, std::cout, std::vector, std::tuple, std::make_tuple;

/*
 * Каждая валюта - узел в графе
 * Рёбра - возможность обмена
 */
class BrokerGraph
{
private:
	// Из какой валюты, в какую, курс, комиссия 
	vector<vector<tuple<int, double, double>>> edges;
public:
	BrokerGraph() {}

	BrokerGraph(int currency_count):
		edges(currency_count)
	{}

	~BrokerGraph() {}

	void addEdge(int cur_from, int cur_to, double r, double c)
	{
		edges[cur_from].push_back(make_tuple(cur_to, r, c));
	}

	int size() const { return edges.size(); }

	const vector<tuple<int, double, double>>
	getNeighbours(int node) const { return edges[node]; }
};

bool isPositiveLoopsExists(const BrokerGraph &g, int start, double V)
{
	double *distance = new double[g.size()];
	for(int i=0; i<g.size(); ++i)
		distance[i] = 0;
	distance[start] = V;

	for(int i=0; i<g.size(); ++i)
		for(int j=0; j<g.size(); ++j)
			for(auto x: g.getNeighbours(j))
			{
				int _to;
				double _r, _c;
				std::tie(_to, _r, _c) = x;

				// distance[_to] = min(distace[_to], distance[j]+w)
				double tmp = (distance[j]-_c)*_r;
				if(distance[_to] < tmp)
					distance[_to] = tmp;
			}

	if(distance[start] > V)
		return true;
	
	bool is_distance_changed = false;
	for(int i=0; i<g.size(); ++i)
		for(auto x: g.getNeighbours(i))
		{
			int _to;
			double _r, _c;
			std::tie(_to, _r, _c) = x;

			double tmp = (distance[i]-_c)*_r;
			if(distance[_to] < tmp)
			{
				is_distance_changed = true;

				// Я художник: я так вижу
				// Я программист: я так больше не могу...
				goto exit_nested_for;
			}
		}
	exit_nested_for:

	delete [] distance;

	return is_distance_changed;
}

int main()
{
	int n, m, start;
	double volume;
	cin >> n >> m >> start >> volume;
	std::cerr << n << m << start << volume << "\n";
	--start;
	
	BrokerGraph g(n);
	for(int i=0; i<m; ++i)
	{
		int a, b;
		double rab, cab, rba, cba;
		cin >> a >> b >> rab >> cab >> rba >> cba;
		std::cerr << a << " " << b << " " << rab << " " << cab << " " << rba << " " << cba << "\n";
		--a; --b;
		g.addEdge(a, b, rab, cab);
		g.addEdge(b, a, rba, cba);
	}

	bool is_exist = isPositiveLoopsExists(g, start, volume);
	cout << ((is_exist)?"YES":"NO") << "\n";

	return 0;
}
