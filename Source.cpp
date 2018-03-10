#include <iostream>
#include "Astar.h"
#include "Floyd-Warshal.h"

/*
	A* pretty much done, turn the nodes into nodes* and at end backtrack parents to find path.
*/
int main()
{
	//initialize classes
	Astar A;
	FloydWarshal B;

	//call classes
	A.main();
	B.main();

	return 0;
}