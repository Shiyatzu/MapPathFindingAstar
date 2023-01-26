#ifndef GRID_H
#define GRID_H

#include <..\freeglut-MSVC-3.0.0-2.mp\freeglut\include\GL\freeglut.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include "stdlib.h"
using namespace std;
struct Node;

class Grid
{
public:
	struct Node {
		int x = NULL, y = NULL;
		int parentX = NULL, parentY = NULL;
		float tCost = FLT_MAX, g = FLT_MAX, h = FLT_MAX;
		int v = 0;
		
		bool isClosed = false;
		bool isOpen = false;

		bool operator< (const Node& a) const {
			return (tCost < a.tCost);
		}
	};

	vector< vector <Node> > map;
	void gridFromMap(string);
	void drawGrid();
	void aStar(int, int, int, int);
	
	list<Node> openList;
	list<Node> closedList;
private:
	int MapStringToGridInt(char);
	bool isValid(int, int);
	bool isBlocked(int, int);
	bool isDestination(int, int, int, int);
	float heuristicValue(int, int, int, int);
	void tracePath(int xDest, int yDest);
};
#endif

