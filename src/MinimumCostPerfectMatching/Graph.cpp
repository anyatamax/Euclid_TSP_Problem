#include <exception>

#include "../../include/MinimumCostPerfectMatching/Graph.h"

class MyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Error: edge does not exist";
    }
};

Graph::Graph(int n, const std::list<std::pair<int, int> > & edges):
	n(n),
	m(edges.size()),
	adjMat(n, std::vector<bool>(n, false)),
	adjList(n),
	edges(),
	edgeIndex(n, std::vector<int>(n, -1))
{
	for(std::list<std::pair<int, int> >::const_iterator it = edges.begin(); it != edges.end(); it++)
	{
		int u = (*it).first;
		int v = (*it).second;

		AddEdge(u, v);
	}
}

std::pair<int, int> Graph::GetEdge(int e) const
{
	if(e > (int)edges.size())
		throw MyException();

	return edges[e];
}

int Graph::GetEdgeIndex(int u, int v) const
{
	if( u > n or
		v > n )
		throw MyException();

	if(edgeIndex[u][v] == -1)
		throw MyException();

	return edgeIndex[u][v];
}

void Graph::AddVertex()
{
	for(int i = 0; i < n; i++)
	{
		adjMat[i].push_back(false);
		edgeIndex[i].push_back(-1);
	}
	n++;
	adjMat.push_back(std::vector<bool>(n, false) );
	edgeIndex.push_back(std::vector<int>(n, -1) );
	adjList.push_back(std::list<int>() );
}

void Graph::AddEdge(int u, int v)
{
	if( u > n or
		v > n )
		throw MyException();

	if(adjMat[u][v]) return;

	adjMat[u][v] = adjMat[v][u] = true;
	adjList[u].push_back(v);
	adjList[v].push_back(u);

	edges.push_back(std::pair<int, int>(u, v));
	edgeIndex[u][v] = edgeIndex[v][u] = m++;
}

const std::list<int> & Graph::AdjList(int v) const
{
	if(v > n)
		throw MyException();

	return adjList[v];
}

const std::vector<std::vector<bool> > & Graph::AdjMat() const
{
	return adjMat;
}
