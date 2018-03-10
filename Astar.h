#pragma once
#include <algorithm> // contains heap
#include <vector>

typedef struct edge
{
	int a;		// start node
	int b;		// dest node
	int weight;	// weight of edge
} edge;

typedef struct node
{
	std::vector<edge> edges;	// list of edges that are in node
	node *parent;				// will be used to backtrack path to source
	int x, y, name;	// x and y coordinates and vertex number
	double cost;				// cost of node via edge
	double guess;				// guess at how far it is to distance
	double total;				// cost + guess 
} node;

class Astar
{
public:
	static int main();
};