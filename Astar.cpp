#include <iostream>
#include <algorithm>
#include <stack>
#include "Astar.h"
#include "Shared.h"

using namespace std;

int compare(node p1,node p2)
{

	if (p1.total > p2.total)
		return 1;

	else
		return 0;
}
double findDistanceToEnd(node currNode, node dest)
{
	//find euclidian distance from current node to end
	double xDist = pow((dest.x - currNode.x), 2);
	double yDist = pow((dest.y - currNode.y), 2);

	return (sqrt(xDist + yDist));
}
vector<node> doAstar(vector<node> nodes, int source, int dest)
{
	/*
	use heap, heaps allow a way for fast retrieval of teh element with highest value at any moment.
	Fits A* because we will be reorganizing nodes and look at nodes with highest potential or value.
	Heap also works with vectors.
	*/
	vector<node> openList;
	vector<node> closedList;

	//initialize source node rest of nodes are infinity
	nodes[source].cost = 0;
	nodes[source].guess = findDistanceToEnd(nodes[source], nodes[dest]); // find distance from node to end
	nodes[source].total = nodes[source].cost + nodes[source].guess;
	nodes[source].parent = NULL;

	openList.push_back(nodes[source]); // push source onto heap
	make_heap(openList.begin(), openList.end(), compare); // make max heap and compare to total, which is cost + estimated guess to end

	while (!openList.empty()) // while list is not empty
	{
		node *currNode = new node;
		*currNode = openList.front(); // set curr node to top of heap aka most promising
		std::pop_heap(openList.begin(), openList.end(), compare); openList.pop_back(); //pop heap

		for (int i = 1; i < currNode->edges.size(); i++) // for all successor nodes, which is all edges minus parent node
		{

			// update node
			nodes[currNode->edges[i].b].cost = currNode->cost + currNode->edges[i].weight;
			nodes[currNode->edges[i].b].guess = findDistanceToEnd(*currNode, nodes[dest]);
			nodes[currNode->edges[i].b].total = nodes[currNode->edges[i].b].cost + nodes[currNode->edges[i].b].guess;
			nodes[currNode->edges[i].b].parent = currNode;

			// push the node onto the heap.
			openList.push_back(nodes[currNode->edges[i].b]); std::push_heap(openList.begin(), openList.end(), compare);


			// if we found destination
			if (currNode->name == nodes[dest].name)
				break;
			
		}
		closedList.push_back(*currNode);
		// if we found destination
		if (currNode->name == nodes[dest].name)
			break;
		
	}
	return closedList;
}
void readFile(vector<node> &nodes, string fileName)
{
	// read file
	fstream inFile;
	node tempNode;
	edge tempEdge;
	char trash;
	int temp;
	inFile.open(fileName);
	nodes.push_back(tempNode); // node 1 starts at index 1 now
	int i = 0;

	while (inFile)
	{
		i++;
		// read in data into temp node, then put node into vector. if end of first line break
		inFile >> tempNode.name;
		inFile >> trash;
		inFile >> tempNode.x;
		inFile >> trash;
		inFile >> tempNode.y;
		cout << " y: " << tempNode.y << endl;
		trash = inFile.peek();
		cout << trash << endl;

		//initialize values to "infinity"
		tempNode.cost = 999;
		tempNode.guess = 999;
		tempNode.total = 999;
		tempNode.parent = NULL;

		nodes.push_back(tempNode);
		nodes[tempNode.name].edges.push_back(tempEdge); // make it so edge 1 is at position 1

		if (trash == '\n')
			break;
	}
	// read in edges

	cout << i << endl;
	while (!inFile.eof())
	{
		inFile >> tempEdge.a;
		inFile >> trash;
		inFile >> tempEdge.b;
		inFile >> trash;
		inFile >> tempEdge.weight;

		nodes[tempEdge.a].edges.push_back(tempEdge); // add edge to vector

		
	}
	inFile.close();

	cout << nodes[1].edges[1].b << endl;
	return;
}
stack<int> backTrackParents(vector<node> x)
{
	stack<int> stk;
	node parent = x[x.size() - 1];

	while (1)
	{
		stk.push(parent.name);
		if (parent.parent == NULL)
			break;
		parent = *parent.parent;

	}
	return stk;
}
void printOutput(vector<node> closedList)
{
	//print output
	int dest = closedList[closedList.size() - 1].name;
	int costDest = closedList[closedList.size() - 1].cost;
	cout << "For point " << closedList[0].name << " to point " << dest << endl;
	cout << "The shortest path is ";

	stack<int> stk = backTrackParents(closedList);
	int size = stk.size();
	while (!stk.empty())
	{
		cout << stk.top() << " ";
		stk.pop();
	}

	cout << endl << "Number of points in path is " << size << endl;
	cout << "Total cost is " << costDest;

	return;
}
void askUser(int &source, int &dest)
{
	// gather user input
	cout << "Enter desired starting point: ";
	cin >> source;
	cout << "Enter desired destination: ";
	cin >> dest;

	return;
}

int Astar::main()
{	
	int source;		// source node
	int dest;		// destination node
	std::vector<node> nodes;
	std::vector<node> closedList;
	string fileName = "test1.txt";

	readFile(nodes, fileName);		//populate node vector with data
	askUser(source, dest);			// get user input
	closedList = doAstar(nodes, source, dest);	// do astar
	printOutput(closedList);			// print output

	return 0;
}

