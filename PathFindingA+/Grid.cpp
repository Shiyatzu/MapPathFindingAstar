#include "Grid.h"
#include <math.h>

void Grid::gridFromMap(string filename)
{
	int xSize, ySize;
	string tempLine;
	ifstream input;
	input.open(filename);
	// test if it opened
	if (!input.is_open()) {
		cout << "The file is not read correctly." << endl;
	} // test if loaded properly
	if (input.good())
	{
		cout << "The file " << filename << " is good" << endl;
		//cout << input.rdbuf(); //writes all file and unloads
	}
	
	// First Line: does nothing
	getline(input, tempLine);
	
	// Second Line: get height
	string temp_str;
	int temp_int;
	getline(input, tempLine);
	stringstream ss1;
	ss1 << tempLine;	
	while (!ss1.eof()) {
		ss1 >> temp_str; //take words into temp_str one by one
		if (stringstream(temp_str) >> temp_int) { //try to convert string to int
			stringstream(temp_str) >> xSize;
			cout << temp_int << " ";
		}
		temp_str = ""; //clear temp string
	}
	// Thrird Line: get width
	getline(input, tempLine);
	stringstream ss2;
	ss2 << tempLine;
	while (!ss2.eof()) {
		ss2 >> temp_str; //take words into temp_str one by one
		if (stringstream(temp_str) >> temp_int) { //try to convert string to int
			stringstream(temp_str) >> ySize;
			cout << temp_int << " \n";
		}
		temp_str = ""; //clear temp string
	}
	cout << xSize << " " << ySize;
	
	// Forth Line does nothing
	getline(input, tempLine);
	
	// REST OF LINES, MAP STARTS HERE: nested loops to create grid
	for (int i = 0; i < xSize; i++)
	{	// Temporary Line
		getline(input, tempLine);
		//cout << tempLine[1] << "\n";
		Node tempNode;
		vector<Node> tempRow;
		for (int j = 0; j < tempLine.size(); j++) // every char in string
		{
			int val = MapStringToGridInt(tempLine[j]);
			tempNode.v = val;
			tempRow.push_back(tempNode);
		} 
		map.push_back(tempRow); // add to vector of vector
	}
	
	// DEBUG SHOW ALL LINES
	/*for (size_t l = 0; l < map.size(); l++)
	{
		for (size_t m = 0; m < map[l].size(); m++)
		{
			//cout << map[l][m].v;
		}
		//cout << endl;
	}*/
}

void Grid::drawGrid()
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			switch (map[i][j].v)
			{
			default: // in case of error
				glColor3f(1.0f, 0.0f, 0.0f); //red
				break;
			case 0: // passable terrain
				glColor3f(1.0f, 1.0f, 1.0f); //white
				break;
			case 1: // out of bounds terrrain
				glColor3f(0.0f, 0.0f, 0.0f); //black
				break;
			case 2: // trees
				glColor3f(0.5f, 0.8f, 0.5f); // Dark Green
				break;
			case 3: // swamp
				glColor3f(0.0f, 0.0f, 1.0f); // Blue
				break;
			case 4: // water
				glColor3f(0.0f, 1.0f, 1.0f); // Light Blue
				break;
			case 5: // isPath
				glColor3f(0.0f, 1.0f, 0.0f); // Green
				break;
			}

			if (map[i][j].isOpen && map[i][j].v != 5) {
				glColor3f(0.9f, 0.9f, 0.9f); // Grey
			}

			glVertex3f(i + 1, -j + 1, 1.0f);
			glVertex3f(i, -j + 1, 1.0f);
			glVertex3f(i, -j, 1.0f);
			glVertex3f(i + 1, -j, 1.0f);
		}
	}
}

int Grid::MapStringToGridInt(char letter) {
	switch (letter)
	{
	default:
		return 9;
		break;
	case '.':
		return 0;
		break;
	case 'G':
		return 0;
		break;
	case '@':
		return 1;
		break;
	case 'O':
		return 1;
		break;
	case 'T':
		return 2;
		break;
	case 'S':
		return 3;
		break;
	case 'W':
		return 4;
		break;
	}
}

bool Grid::isValid(int row, int col) {  // Returns true if row and column is in range
	//cout << "\n" << row << " " << col << " " << map.size() << " " << map[0].size();
	return (row >= 0) && (row < map.size()) && (col >= 0) && (col < map[0].size());
}

bool Grid::isBlocked(int row, int col) // Returns false if the cell is blocked 
{ 
	//cout << "\n" << row << " " << col << " " << map[row][col].v;
	if (map[row][col].v == 1 || map[row][col].v == 2 || 
		map[row][col].v == 3 || map[row][col].v == 4) {
		//cout << "\nBlocked";
		return true;
	}
	else
		return false;
}

bool Grid::isDestination(int row, int col, int xDest, int yDest) // Check if Node is destination
{
	if (row == xDest && col == yDest)
		return (true);
	else
		return (false);
}

float Grid::heuristicValue(int row, int col, int xDest, int yDest)
{
	float x = pow(xDest - row, 2.0);
	float y = pow(yDest - col, 2.0);
	return sqrt(x + y);
	//return abs(row - xDest) + abs(col - yDest);
}

void Grid::tracePath(int xDest, int yDest)
{
	int row = xDest;
	int col = yDest;

	while (!(map[row][col].parentX == row && map[row][col].parentY == col))
	{
		map[row][col].v = 5;
		int temp_row = map[row][col].parentX;
		int temp_col = map[row][col].parentY;
		row = temp_row;
		col = temp_col;
	}
	return;
}

void Grid::aStar(int xStart, int yStart, int xDest, int yDest) {
	if (!isValid(xStart, yStart)) {
		cout << "\n Start is invalid";
		return;
	}

	if (!isValid(xDest, yDest)) {
		cout << "\n Destination is invalid";
		return;
	}
	
	if (isBlocked(xStart, yStart)) {
		cout << "\n Start is blocked";
		return;
	}

	if (isBlocked(xDest, yDest)) {
		cout << "\n Destination is blocked";
		return;
	}
	
	if (isDestination(xStart, yStart, xDest, yDest) == true)
	{
		printf("\n We are already at the destination");
		return;
	}

	// initialise values for starting node and add to open list
	int xCurrent = xStart, yCurrent = yStart;
	map[xCurrent][yCurrent].tCost = 0.0;
	map[xCurrent][yCurrent].g = 0.0;
	map[xCurrent][yCurrent].h = 0.0;
	map[xCurrent][yCurrent].v = 9.0;
	map[xCurrent][yCurrent].parentX = xCurrent;
	map[xCurrent][yCurrent].parentY = yCurrent;
	map[xCurrent][yCurrent].x = xCurrent;
	map[xCurrent][yCurrent].y = yCurrent;
	
	//cout << "\n starting " << xCurrent << "," << yCurrent;
	openList.push_back( map[xCurrent][yCurrent] );


	bool foundDest = false;

	while (!openList.empty())
	{   
		//cout << "\neval " << openList.front().x << "," << openList.front().y << " ";
		xCurrent = openList.front().x;
		yCurrent = openList.front().y;
		openList.front().isClosed = true;
		//remove from open list
		openList.erase(openList.begin());
		// add to closed list
		closedList.push_back( map[xCurrent][yCurrent] );

		float gNew, hNew, tNew;
		// NORTH 
		if (isValid(xCurrent - 1, yCurrent) == true)
		{
			// If the destination cell is the same as the current successor 
			if (isDestination(xCurrent - 1, yCurrent, xDest, yDest) == true)
			{
				// Set the Parent of the destination cell 
				map[xCurrent - 1][yCurrent].parentX = xCurrent;
				map[xCurrent - 1][yCurrent].parentY = yCurrent;
				cout << "\nThe destination cell is found";
				tracePath(xDest, yDest);
				foundDest = true;
				return;
			} // if not blocked or closed
			else if (!isBlocked(xCurrent - 1, yCurrent) || map[xCurrent - 1][yCurrent].isClosed) {
				gNew = map[xCurrent][yCurrent].g + 1.0f;
				hNew = heuristicValue(xCurrent - 1, yCurrent, xDest, yDest);
				tNew = gNew + hNew;
				//cout << "\n North not blocked";
				// if new node
				if (map[xCurrent - 1][yCurrent].tCost == FLT_MAX) {
					map[xCurrent - 1][yCurrent].tCost = tNew;
					map[xCurrent - 1][yCurrent].g = gNew;
					map[xCurrent - 1][yCurrent].h = hNew;
					map[xCurrent - 1][yCurrent].isOpen = true;
					map[xCurrent - 1][yCurrent].parentX = xCurrent;
					map[xCurrent - 1][yCurrent].parentY = yCurrent;
					map[xCurrent - 1][yCurrent].x = xCurrent - 1;
					map[xCurrent - 1][yCurrent].y = yCurrent;
					//cout << "\n adding " << xCurrent - 1  << "," << yCurrent;
					openList.push_back(map[xCurrent - 1][yCurrent]);
					//cout << "\n test " << openList.back().x << "," << openList.back().y;
				} // if older node but better total Cost
				else if (map[xCurrent - 1][yCurrent].tCost > tNew) {
					map[xCurrent - 1][yCurrent].parentX = xCurrent;
					map[xCurrent - 1][yCurrent].parentY = yCurrent;
					map[xCurrent - 1][yCurrent].tCost = tNew;
					map[xCurrent - 1][yCurrent].g = gNew;
					map[xCurrent - 1][yCurrent].h = hNew;
				}
			}
			//!(find(closedList.begin(), closedList.end(), map[xCurrent - 1][yCurrent]) != closedList.end()

		} // END OF NORTH
		// SOUTH
		if (isValid(xCurrent + 1, yCurrent) == true)
		{
			// If the destination cell is the same as the current successor 
			if (isDestination(xCurrent + 1, yCurrent, xDest, yDest) == true)
			{
				// Set the Parent of the destination cell 
				map[xCurrent + 1][yCurrent].parentX = xCurrent;
				map[xCurrent + 1][yCurrent].parentY = yCurrent;
				cout << "\nThe destination cell is found";
				tracePath(xDest, yDest);
				foundDest = true;
				return;
			}
			else if (!isBlocked(xCurrent + 1, yCurrent) || map[xCurrent + 1][yCurrent].isClosed) {
				gNew = map[xCurrent][yCurrent].g + 1.0f;
				hNew = heuristicValue(xCurrent + 1, yCurrent, xDest, yDest);
				tNew = gNew + hNew;
				//cout << "\n South not blocked";

				if (map[xCurrent + 1][yCurrent].tCost == FLT_MAX) {
					map[xCurrent + 1][yCurrent].tCost = tNew;
					map[xCurrent + 1][yCurrent].g = gNew;
					map[xCurrent + 1][yCurrent].h = hNew;
					map[xCurrent + 1][yCurrent].isOpen = true;
					map[xCurrent + 1][yCurrent].parentX = xCurrent;
					map[xCurrent + 1][yCurrent].parentY = yCurrent;
					map[xCurrent + 1][yCurrent].x = xCurrent + 1;
					map[xCurrent + 1][yCurrent].y = yCurrent;
					openList.push_back(map[xCurrent + 1][yCurrent]);
				}
				else if (map[xCurrent + 1][yCurrent].tCost > tNew) {
					map[xCurrent + 1][yCurrent].parentX = xCurrent;
					map[xCurrent + 1][yCurrent].parentY = yCurrent;
					map[xCurrent + 1][yCurrent].tCost = tNew;
					map[xCurrent + 1][yCurrent].g = gNew;
					map[xCurrent + 1][yCurrent].h = hNew;
				}
			}
			else {
				//cout << "\n South blocked";
			}
			
		}// END OF SOUTH
		// EAST
		if (isValid(xCurrent, yCurrent + 1) == true)
		{
			// If the destination cell is the same as the current successor 
			if (isDestination(xCurrent, yCurrent + 1, xDest, yDest) == true)
			{
				// Set the Parent of the destination cell 
				map[xCurrent][yCurrent + 1].parentX = xCurrent;
				map[xCurrent][yCurrent + 1].parentY = yCurrent;
				cout << "\nThe destination cell is found";
				tracePath(xDest, yDest);
				foundDest = true;
				return;
			}
			else if (!isBlocked(xCurrent, yCurrent + 1) || map[xCurrent][yCurrent + 1].isClosed) {
				gNew = map[xCurrent][yCurrent].g + 1.0f;
				hNew = heuristicValue(xCurrent, yCurrent + 1, xDest, yDest);
				tNew = gNew + hNew;

				if (map[xCurrent][yCurrent + 1].tCost == FLT_MAX) {
					map[xCurrent][yCurrent + 1].tCost = tNew;
					map[xCurrent][yCurrent + 1].g = gNew;
					map[xCurrent][yCurrent + 1].h = hNew;
					map[xCurrent][yCurrent + 1].isOpen = true;
					map[xCurrent][yCurrent + 1].parentX = xCurrent;
					map[xCurrent][yCurrent + 1].parentY = yCurrent;
					map[xCurrent][yCurrent + 1].x = xCurrent;
					map[xCurrent][yCurrent + 1].y = yCurrent + 1;
					openList.push_back(map[xCurrent][yCurrent + 1]);
				}
				else if (map[xCurrent][yCurrent + 1].tCost > tNew) {
					map[xCurrent][yCurrent + 1].parentX = xCurrent;
					map[xCurrent][yCurrent + 1].parentY = yCurrent;
					map[xCurrent][yCurrent + 1].tCost = tNew;
					map[xCurrent][yCurrent + 1].g = gNew;
					map[xCurrent][yCurrent + 1].h = hNew;
				}
			}
		}// END OF EAST
		// WEST
		if (isValid(xCurrent, yCurrent - 1) == true)
		{
			// If the destination cell is the same as the current successor 
			if (isDestination(xCurrent, yCurrent - 1, xDest, yDest) == true)
			{
				// Set the Parent of the destination cell 
				map[xCurrent][yCurrent - 1].parentX = xCurrent;
				map[xCurrent][yCurrent - 1].parentY = yCurrent;
				cout << "\nThe destination cell is found";
				tracePath(xDest, yDest);
				foundDest = true;
				return;
			}
			else if (!isBlocked(xCurrent, yCurrent - 1) || map[xCurrent][yCurrent - 1].isClosed) {
				gNew = map[xCurrent][yCurrent].g + 1.0f;
				hNew = heuristicValue(xCurrent, yCurrent - 1, xDest, yDest);
				tNew = gNew + hNew;

				if (map[xCurrent][yCurrent - 1].tCost == FLT_MAX) { //if never checked
					map[xCurrent][yCurrent - 1].tCost = tNew;
					map[xCurrent][yCurrent - 1].g = gNew;
					map[xCurrent][yCurrent - 1].h = hNew;
					map[xCurrent][yCurrent - 1].isOpen = true;
					map[xCurrent][yCurrent - 1].parentX = xCurrent;
					map[xCurrent][yCurrent - 1].parentY = yCurrent;
					map[xCurrent][yCurrent - 1].x = xCurrent;
					map[xCurrent][yCurrent - 1].y = yCurrent - 1;

					openList.push_back(map[xCurrent][yCurrent - 1]);
				}
				else if (map[xCurrent][yCurrent - 1].tCost > tNew) { // if checked but better value
					map[xCurrent][yCurrent - 1].parentX = xCurrent;
					map[xCurrent][yCurrent - 1].parentY = yCurrent;
					map[xCurrent][yCurrent - 1].tCost = tNew;
					map[xCurrent][yCurrent - 1].g = gNew;
					map[xCurrent][yCurrent - 1].h = hNew;
				}
			}
		}// END OF WEST
		// NORTH EAST
		if (isValid(xCurrent - 1, yCurrent + 1) == true)
		{
			// If the destination cell is the same as the current successor 
			if (isDestination(xCurrent - 1, yCurrent + 1, xDest, yDest) == true)
			{
				// Set the Parent of the destination cell 
				map[xCurrent - 1][yCurrent + 1].parentX = xCurrent;
				map[xCurrent - 1][yCurrent + 1].parentY = yCurrent;
				cout << "\nThe destination cell is found";
				tracePath(xDest, yDest);
				foundDest = true;
				return;
			} // if not blocked
			else if (!isBlocked(xCurrent - 1, yCurrent + 1) || map[xCurrent - 1][yCurrent + 1].isClosed) {
				gNew = map[xCurrent][yCurrent].g + 1.414f;
				hNew = heuristicValue(xCurrent - 1, yCurrent + 1, xDest, yDest);
				tNew = gNew + hNew;
				//cout << "\n North east not blocked";
				if (map[xCurrent - 1][yCurrent + 1].tCost == FLT_MAX) {
					map[xCurrent - 1][yCurrent + 1].tCost = tNew;
					map[xCurrent - 1][yCurrent + 1].g = gNew;
					map[xCurrent - 1][yCurrent + 1].h = hNew;
					map[xCurrent - 1][yCurrent + 1].isOpen = true;
					map[xCurrent - 1][yCurrent + 1].parentX = xCurrent;
					map[xCurrent - 1][yCurrent + 1].parentY = yCurrent;
					map[xCurrent - 1][yCurrent + 1].x = xCurrent - 1;
					map[xCurrent - 1][yCurrent + 1].y = yCurrent + 1;
					//cout << "\n adding " << xCurrent - 1 << "," << yCurrent + 1;
					openList.push_back(map[xCurrent - 1][yCurrent + 1]);
					//cout << "\n test " << openList.back().x << "," << openList.back().y;
				}
				else if (map[xCurrent - 1][yCurrent + 1].tCost > tNew) {
					map[xCurrent - 1][yCurrent + 1].parentX = xCurrent;
					map[xCurrent - 1][yCurrent + 1].parentY = yCurrent;
					map[xCurrent - 1][yCurrent + 1].tCost = tNew;
					map[xCurrent - 1][yCurrent + 1].g = gNew;
					map[xCurrent - 1][yCurrent + 1].h = hNew;
				}
			}
		} // END OF NORTH EAST
		// NORTH WEST
		if (isValid(xCurrent - 1, yCurrent - 1) == true)
		{
			// If the destination cell is the same as the current successor 
			if (isDestination(xCurrent - 1, yCurrent - 1, xDest, yDest) == true)
			{
				// Set the Parent of the destination cell 
				map[xCurrent - 1][yCurrent - 1].parentX = xCurrent;
				map[xCurrent - 1][yCurrent - 1].parentY = yCurrent;
				cout << "\nThe destination cell is found";
				tracePath(xDest, yDest);
				foundDest = true;
				return;
			} // if not blocked
			else if (!isBlocked(xCurrent - 1, yCurrent - 1) || map[xCurrent - 1][yCurrent - 1].isClosed) {
				gNew = map[xCurrent][yCurrent].g + 1.414f;
				hNew = heuristicValue(xCurrent - 1, yCurrent - 1, xDest, yDest);
				tNew = gNew + hNew;
				//cout << "\n North not blocked";
				if (map[xCurrent - 1][yCurrent - 1].tCost == FLT_MAX) {
					map[xCurrent - 1][yCurrent - 1].tCost = tNew;
					map[xCurrent - 1][yCurrent - 1].g = gNew;
					map[xCurrent - 1][yCurrent - 1].h = hNew;
					map[xCurrent - 1][yCurrent - 1].isOpen = true;
					map[xCurrent - 1][yCurrent - 1].parentX = xCurrent;
					map[xCurrent - 1][yCurrent - 1].parentY = yCurrent;
					map[xCurrent - 1][yCurrent - 1].x = xCurrent - 1;
					map[xCurrent - 1][yCurrent - 1].y = yCurrent - 1;
					//cout << "\n adding " << xCurrent - 1 << "," << yCurrent - 1;
					openList.push_back(map[xCurrent - 1][yCurrent - 1]);
					//cout << "\n test " << openList.back().x << "," << openList.back().y;
				}
				else if (map[xCurrent - 1][yCurrent - 1].tCost > tNew) {
					map[xCurrent - 1][yCurrent - 1].parentX = xCurrent;
					map[xCurrent - 1][yCurrent - 1].parentY = yCurrent;
					map[xCurrent - 1][yCurrent - 1].tCost = tNew;
					map[xCurrent - 1][yCurrent - 1].g = gNew;
					map[xCurrent - 1][yCurrent - 1].h = hNew;
				}
			}
		} // END OF NORTH WEST
		// SOUTH EAST
		if (isValid(xCurrent + 1, yCurrent + 1) == true)
		{
			// If the destination cell is the same as the current successor 
			if (isDestination(xCurrent + 1, yCurrent + 1, xDest, yDest) == true)
			{
				// Set the Parent of the destination cell 
				map[xCurrent + 1][yCurrent + 1].parentX = xCurrent;
				map[xCurrent + 1][yCurrent + 1].parentY = yCurrent;
				cout << "\nThe destination cell is found";
				tracePath(xDest, yDest);
				foundDest = true;
				return;
			} // if not blocked
			else if (!isBlocked(xCurrent + 1, yCurrent + 1) || map[xCurrent + 1][yCurrent + 1].isClosed) {
				gNew = map[xCurrent][yCurrent].g + 1.414f;
				hNew = heuristicValue(xCurrent + 1, yCurrent + 1, xDest, yDest);
				tNew = gNew + hNew;
				//cout << "\n North not blocked";
				if (map[xCurrent + 1][yCurrent + 1].tCost == FLT_MAX) {
					map[xCurrent + 1][yCurrent + 1].tCost = tNew;
					map[xCurrent + 1][yCurrent + 1].g = gNew;
					map[xCurrent + 1][yCurrent + 1].h = hNew;
					map[xCurrent + 1][yCurrent + 1].isOpen = true;
					map[xCurrent + 1][yCurrent + 1].parentX = xCurrent;
					map[xCurrent + 1][yCurrent + 1].parentY = yCurrent;
					map[xCurrent + 1][yCurrent + 1].x = xCurrent + 1;
					map[xCurrent + 1][yCurrent + 1].y = yCurrent + 1;
					//cout << "\n adding " << xCurrent + 1 << "," << yCurrent + 1;
					openList.push_back(map[xCurrent + 1][yCurrent + 1]);
					//cout << "\n test " << openList.back().x << "," << openList.back().y;
				}
				else if (map[xCurrent + 1][yCurrent + 1].tCost > tNew) {
					map[xCurrent + 1][yCurrent + 1].parentX = xCurrent;
					map[xCurrent + 1][yCurrent + 1].parentY = yCurrent;
					map[xCurrent + 1][yCurrent + 1].tCost = tNew;
					map[xCurrent + 1][yCurrent + 1].g = gNew;
					map[xCurrent + 1][yCurrent + 1].h = hNew;
				}
			}
		} // END OF SOUTH EAST
		// SOUTH WEST
		if (isValid(xCurrent + 1, yCurrent - 1) == true)
		{
			// If the destination cell is the same as the current successor 
			if (isDestination(xCurrent + 1, yCurrent - 1, xDest, yDest) == true)
			{
				// Set the Parent of the destination cell 
				map[xCurrent + 1][yCurrent - 1].parentX = xCurrent;
				map[xCurrent + 1][yCurrent - 1].parentY = yCurrent;
				cout << "\nThe destination cell is found";
				tracePath(xDest, yDest);
				foundDest = true;
				return;
			} // if not blocked
			else if (!isBlocked(xCurrent + 1, yCurrent - 1) || map[xCurrent + 1][yCurrent - 1].isClosed) {
				gNew = map[xCurrent][yCurrent].g + 1.414f;
				hNew = heuristicValue(xCurrent + 1, yCurrent - 1, xDest, yDest);
				tNew = gNew + hNew;
				//cout << "\n North not blocked";
				if (map[xCurrent + 1][yCurrent - 1].tCost == FLT_MAX) {
					map[xCurrent + 1][yCurrent - 1].tCost = tNew;
					map[xCurrent + 1][yCurrent - 1].g = gNew;
					map[xCurrent + 1][yCurrent - 1].h = hNew;
					map[xCurrent + 1][yCurrent - 1].isOpen = true;
					map[xCurrent + 1][yCurrent - 1].parentX = xCurrent;
					map[xCurrent + 1][yCurrent - 1].parentY = yCurrent;
					map[xCurrent + 1][yCurrent - 1].x = xCurrent + 1;
					map[xCurrent + 1][yCurrent - 1].y = yCurrent - 1;
					//cout << "\n adding " << xCurrent + 1 << "," << yCurrent - 1;
					openList.push_back(map[xCurrent + 1][yCurrent - 1]);
					//cout << "\n test " << openList.back().x << "," << openList.back().y;
				}
				else if (map[xCurrent + 1][yCurrent - 1].tCost > tNew) {
					map[xCurrent + 1][yCurrent - 1].parentX = xCurrent;
					map[xCurrent + 1][yCurrent - 1].parentY = yCurrent;
					map[xCurrent + 1][yCurrent - 1].tCost = tNew;
					map[xCurrent + 1][yCurrent - 1].g = gNew;
					map[xCurrent + 1][yCurrent - 1].h = hNew;
				}
			}
		} // END OF SOUTH WEST

		openList.sort([](const Node & a, const Node & b) { return a.tCost < b.tCost; });
		
		/*for (auto v : openList) {
			cout << "\n" << v.x << " " << v.y;
			cout << "\n" << v.g << " " << v.h << " " << v.tCost;
		}*/
	}

	if (foundDest == false) {
		cout << "\nFailed to find the Destination Cell\n";
		return;
	}
}