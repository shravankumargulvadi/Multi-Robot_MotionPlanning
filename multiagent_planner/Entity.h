#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "fssimplewindow.h"
#include "Maze.h"
#include <chrono>
#include <algorithm>
#include "yssimplesound.h"
#include <queue>
#include<windows.h>



using namespace std;

class Entity
{

	public:
	vector<cell> optimalPath;
	vector<vector<int>> path;

	int entity_row, entity_col,goal_row,goal_col;
	Maze *myMaze=nullptr;
	void setMaze(Maze& aMaze);
	void move( int direction);
	void paint();
	bool reset(); // places entity at maze start 
	bool reachedGoal();
	bool isNavigable(int row, int col);
	void celebrate();
	void drawEntity(int vert_x, int vert_y,int inflation, int r, int g, int b);
	void killEntity();
	void erase();
	void findShortestPath();
	vector<cell> getSuccessors(cell currCell, bool visited[][40]);
	void  selfPlay();
	void drawCrumbs();


};
