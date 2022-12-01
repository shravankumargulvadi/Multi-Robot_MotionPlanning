
#include<vector>
#include<string>
#include<iostream>
#include"HighLevelSolver.h"


using namespace std;



std::vector<std::string> split(std::string s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}
	return result;
}

Map Multi_readMap(std::string filename) {

	Map map; // Grid map
	int row, col;

	std::ifstream infile(filename);
	std::string line;

	getline(infile, line);

	auto splitted = split(line, ' ');
	row = std::stoi(splitted[0]);
	col = std::stoi(splitted[1]);

	std::cout << "row" << row << " " << "col" << col;

	getline(infile, line);

	// split second line to get obstacles
	std::istringstream k(line);
	std::string s;

	std::vector<std::vector<Cell>> cells;

	for (int i = 0; i < row; i++) {

		std::vector<Cell> row;
		for (int j = 0; j < col; j++) {
			row.emplace_back(Cell(i, j));
		}
		cells.emplace_back(row);
	};

	while (getline(k, s, ' ')) {

		int obstacleIndex = std::stoi(s);
		int x = obstacleIndex % row;
		int y = obstacleIndex / row;
		std::cout << "obs " << x << " " << y << std::endl;
		cells[x][y].isObstacle = true;
	}

	// next lines are agents. startIndex endIndex
	int start, end;

	std::vector<Agent> agents;

	int agentID = 0;
	while (infile >> start >> end) {

		int startX = start % row;
		int startY = start / row;

		int endX = end % row;
		int endY = end / row;

		Agent agent(agentID);
		agent.start = Cell(startX, startY);
		agent.end = Cell(endX, endY);
		std::cout << "start: " << startX << startY;
		std::cout << " end: " << endX << endY << "\n";
		agents.emplace_back(agent);
		agentID++;
	}

	map.cells = cells;
	map.agents = agents;
	return map;
}

void Multi_printMap(Map map) {
	for (int i = 0; i < map.cells.size(); i++) {
		for (int j = 0; j < map.cells[0].size(); j++) {
			if (map.cells[j][i].isObstacle)
				std::cout << "X";
			else
				std::cout << "_";
		}
		std::cout << std::endl;
	}
}

void Multi_printAgents(Map map) {
	for (int i = 0; i < map.agents.size(); i++) {
		std::cout << map.agents[i].start.y;

	}
}

void Multi_testLowLevel(Map map) {
	LowLevelSolver solver;
	auto x = solver.findOptimalPaths(std::vector<Constraint>(), map);

	std::cout << x.size();
	for (int i = 0; i < x.size(); i++) {
		for (int j = 0; j < x[0].size(); j++) {
			std::cout << x[i][j].x << "," << x[i][j].y << std::endl;
		}
	}

}

unsigned int Factorial(unsigned int number) {
	return number <= 1 ? number : Factorial(number - 1) * number;
}

void Multi_printSolution(std::vector<std::vector<Cell>> optimalPaths, vector<Entity>& entity) {
	int i = 0;

	for (auto currPath : optimalPaths) {
		std::cout << "Optimal path of agent \n";

		for (auto cell : currPath) {
			std::cout << cell.x << cell.y << "\n";
			vector<int> step = { cell.y,cell.x };
			entity[i].path.insert(entity[i].path.begin(), step);

		}
		i += 1;
	}
}

