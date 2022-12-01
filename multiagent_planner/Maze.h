#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
using namespace std;

struct cell
{
	int row, col;
};
class Maze
{
private:
	
	int currRow=0;
	
	
	


public:
	string startRow, startCol, goalRow, goalCol, totalCols;
	vector<vector<string>> startVec, goalVec;
	int quadSize = 20;
	
	
	int maxRow;
	string map[40][40];

	void readFile(std::ifstream& input);
	void printMaze();
	void paint();
	void makeQuad(int vert_x, int vert_y,int r,int g,int b);
	void printMessage();
	void changeMapState(int mouseX, int mouseY, int key);
	std::ostream& fileDump(std::ostream& os);
	void mapToPlanner();
	

};
