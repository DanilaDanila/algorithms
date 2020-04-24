#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>

using std::cin, std::cout, std::string, std::vector, std::bitset, std::reverse;

/*
 *	Ребро из A в B существует, если
 *	из A в B есть путь
 */
class CohesionGraph
{
private:
	vector<bitset<1000>> edges;
public:
	CohesionGraph() {}
	
	CohesionGraph(int nodes_count):
		edges(nodes_count)
	{}

	~CohesionGraph() {}

	void addEdges(int node, string str)
	{
		reverse(str.begin(), str.end());
		edges[node] = bitset<1000>(str);
	}

	// Применение к графу (слегка модернизированного)
	// алгоритма Флойда
	void buildGraph()
	{
		/*
		for(int i=0; i<edges.size(); ++i)
			for(int j=0; j<edges.size(); ++j)
				for(int k=0; k<edges.size(); ++k)
					if(edges[j][k] == 0)
						edges[j][k] = (edges[j][i]==1) && (edges[i][k]==1);
		*/
		for(int i=0; i<edges.size(); ++i)
			for(int j=0; j<edges.size(); ++j)
				if(edges[j][i])
					edges[j] |= edges[i];
	}

friend std::ostream& operator<<(std::ostream &, const CohesionGraph&);
};

std::ostream &operator<<(std::ostream &out, const CohesionGraph &g)
{
	short zero_one[2] = {0, 1};
	for(int i=0; i<g.edges.size(); ++i)
	{
		for(int j=0; j<g.edges.size()/*Ибо он квадратный*/; ++j)
			out << zero_one[g.edges[i][j]];
		out << '\n';
	}
	return out;	
}

int main()
{
	int n;
	cin >> n;

	CohesionGraph g(n);
	for(int i=0; i<n; ++i)
	{
		string tmp;
		cin >> tmp;
		g.addEdges(i, tmp);
	}

	g.buildGraph();

	cout << g;
	return 0;
}
