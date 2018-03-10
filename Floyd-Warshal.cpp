#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <ctime>
#include <chrono>


using namespace std;

#define infinity 999

vector< vector<int> > readFile(string fileName)
{
	// read file
	fstream inFile;
	vector<int> axis;
	axis.push_back(sizeof(int));

	char trash;
	int tempx;
	int tempy;
	int weight;
	int sTemp, oldpos;
	inFile.open(fileName);

	// read in nodes
	while (inFile)
	{
		// read in data into temp node, then put node into vector. if end of first line break
		inFile >> tempx;
		axis.push_back(tempx);
		inFile >> trash;
		inFile >> tempy;
		inFile >> trash;
		inFile >> tempy;

		oldpos = inFile.tellg();
		inFile >> sTemp;
		if (sTemp != (tempx + 1))
		{
			cout << "STOP!" << endl;
			inFile.seekg(oldpos);
			break;
		}
		inFile.seekg(oldpos);
	}

	vector< vector<int> > graph(axis.size(), vector <int>(axis.size(), infinity)); // make graph with size of amount of nodes and default size to 'infinity' or 999
																				   // read in edges
	while (!inFile.eof())
	{
		inFile >> tempx;
		inFile >> trash;
		inFile >> tempy;
		inFile >> trash;
		inFile >> weight;

		graph[tempx][tempy] = weight;

	}
	inFile.close();
	return graph;
}
void askUser(int &source, int &dest, int size)
{
	// gather user input
	cout << "Enter desired starting point (from 1 to " << size - 1 << "): ";
	cin >> source;
	cout << "Enter desired destination (from 1 to " << size - 1 << "): ";
	cin >> dest;

	if (source > size - 1 || source <= 0 || dest > size - 1 || dest <= 0)
	{
		cout << "One (or both) of the verticies doesn't exist. Try again." << endl;
		askUser(source, dest, size);
	}
	return;
}
void printOutput(vector< vector<int> > &graph, stack<int> shortestPath, int source, int dest)
{
	//print output
	int size = shortestPath.size();
	cout << "For point " << source << " to point " << dest << endl;
	cout << "The shortest path is ";

	while (!shortestPath.empty())
	{
		cout << shortestPath.top() << " ";
		shortestPath.pop();
	}

	cout << endl << "Number of points in path is " << size << endl;
	cout << "Total cost is " << graph[source][dest];

	return;
}
stack<int> doFloydWarshal(vector< vector<int> > &graph, int source, int dest)
{
	int N = graph.size(); // size of rows/columns
						  // IF SOMETHING BREAKS PUT NULL INSTEAD OF 0
	vector< vector<int> > predGraph(graph.size(), vector <int>(graph.size(), 0)); // predecessor graph
	vector< vector<int> > prevGraph(graph.size(), vector <int>(graph.size(), 0)); // previous graph
	stack<int> sp;

	for (int k = 1; k < N + 1; k++)
	{
		prevGraph = graph;
		for (int i = 1; i < N; i++)
		{
			graph[i][i] = 0;
			for (int j = 1; j < N; j++)
			{
				int a = prevGraph[i][k - 1];
				int b = prevGraph[k - 1][j];
				int c = a + b;
				int d = prevGraph[i][j];
				if ((c < d) && (a != infinity) && (b != infinity))
					graph[i][j] = c;


				// update predecessor graph	
				if (k == 1)
				{
					if ((i == j) || (graph[i][j] == infinity))
						predGraph[i][j] = 0;

					if ((i != j) && (graph[i][j] < infinity))
						predGraph[i][j] = i;
				}
				else
				{
					int x = prevGraph[i][j];
					int y = prevGraph[i][k - 1] + prevGraph[k - 1][j];
					if (x <= y)
						predGraph[i][j] = predGraph[i][j]; //stays same

					else
						predGraph[i][j] = predGraph[k - 1][j]; //updates
				}
			}
		}
	}
	//cycle back through predGraph and find shortest path

	sp.push(dest);
	while (source != dest)
	{
		dest = predGraph[source][dest];
		if (dest == 0)
		{
			sp.pop();
			sp.push(source);
			break;
		}
		sp.push(dest);
	}

	return sp;
}

bool again()
{
	char answer;

	cout << "Run program again (y/n)?" << endl;
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		return true;
	}
	else
		return false;
}

int main()
{
	int source;		// source node
	int dest;		// destination node
	vector< vector<int> > graph;
	stack<int> shortestPath;
	string fileName = "test1.txt";

	graph = readFile(fileName);		//populate node vector with data

	do
	{
		askUser(source, dest, graph.size());			// get user input
		chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now(); // compute time 1
		shortestPath = doFloydWarshal(graph, source, dest);// do floydWarshal
		chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now(); // compute time 2

		printOutput(graph, shortestPath, source, dest);	// print output

		chrono::duration<double> time_span = chrono::duration_cast <chrono::duration<double >> (t2 - t1);
		double diff = time_span.count();
		cout << endl << "time: " << diff << endl;
	} while (again());
	return 0;
}