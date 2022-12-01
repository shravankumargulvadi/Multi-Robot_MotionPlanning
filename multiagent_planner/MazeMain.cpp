// 24-780 Engineering Computation Assignment 6 
// ps02_dice_sgulvadi.cpp : Find shortest path and more functionalities to the Maze game
// Date:10/19/2022

#include "Maze.h"
#include "Entity.h"
#include <chrono>
#include "LowLevelSolver.h"
#include "HighLevelSolver.h"



int Menu(string &customFileName)
{//ask user to choose level and return it
	string userAns;
	int level;
	cout << "Do you want to load a custom level? Y/N :";
	cin >> userAns;
	if (userAns == "N" || userAns == "n") //Load standard levels if not custom levels
	{
		cout << "Choose level to play: " << endl << "Press '1' for level-1" << endl;
		cout << "Press '2' for level-2" << endl << "Press '3' for level-3" << endl << "Level: ";
		cin >> level;
		return level;
	}
	else if (userAns == "Y" || userAns == "y")//if user chooses custom level take filename
	{
		cout << "Enter the file name : ";
		cin >> customFileName;
		std::ifstream inFile4(customFileName);

		cout << endl << "chosen file name is" <<" "<< customFileName<<endl;
		level = -1;
		return level;
	}
	else//wrong key typed
	{
		cout << "Invalid input" << endl;
		Menu(customFileName);
	}
}

std::ostream& operator<<(std::ostream& os, const Maze& aMaze)
{//overload '<<' for osstream
	os << "Start: " << aMaze.startRow << " " << aMaze.startCol << endl;
	os << "End: " << aMaze.goalRow << " " << aMaze.goalCol << endl;
	os << "Map Begin: " << aMaze.totalCols << endl;
	for (int i = 0; i <= aMaze.maxRow; i++) {
		for (int j = 0; j <= stoi(aMaze.totalCols); j++)
			os << aMaze.map[i][j] << " ";
		os << endl;
	}
	os << "Map End: " << endl;
	return os;
}

void saveFile(Maze& theMaze)
{// function to savefile,
	string fileToSave;
	cout << "Enter the name of the file to save >> ";
	cin >> fileToSave;
	if (fileToSave.length() > 0)
	{
		ofstream outfile(fileToSave);
		if (outfile.is_open())
		{
			outfile << theMaze;//using overloaded '<<' operator
			outfile.close();
		}
		else {
			cout << "could not open the file";
		}
	}
}




std::vector<std::string> split(std::string s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss(s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}
	return result;
}

Map readMap(std::string filename) {

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

void printMap(Map map) {
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

void printAgents(Map map) {
	for (int i = 0; i < map.agents.size(); i++) {
		std::cout << map.agents[i].start.y;

	}
}

void testLowLevel(Map map) {
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

void printSolution(std::vector<std::vector<Cell>> optimalPaths, vector<Entity> &entity) {
	int i = 0;

	for (auto currPath : optimalPaths) {
		std::cout << "Optimal path of agent \n";
		
		for (auto cell : currPath) {
			std::cout << cell.x << cell.y << "\n";
			vector<int> step = { cell.y,cell.x };
			entity[i].path.insert(entity[i].path.begin(),step);

		}
		i += 1;
	}
}



void Multi_selfPlay(Entity entity)
{
	vector<int> nextStep = entity.path.back();
	while (!(nextStep[0] == (stoi(entity.myMaze->goalRow) - 1) && nextStep[1] == (stoi(entity.myMaze->goalCol) - 1)))
	{// execute optimal path until we reach goal


		vector<int> nextStep = entity.path.back();

		entity.path.pop_back();
		glFlush();
		entity.drawCrumbs();// drawcrumbs to trace path
		entity.entity_row = nextStep[0] + 1;
		entity.entity_col = nextStep[1] + 1;
		entity.paint();
		break;

		Sleep(100);
		if (nextStep[0] == entity.goal_row - 1 && nextStep[1] == entity.goal_col - 1)
		{
			break;

		}


	}

}










int main()
{
	
	int key;
	int mouseEvent, leftButton, middleButton, rightButton, locX, locY;
	
	string customFileName;
	int level = Menu(customFileName); //display menu and read level
	Entity entity1, entity2;
	vector<Entity> entity;
	
	FsOpenWindow(5, 5, 800, 800, 0, "Maze Game");

	while (level != 0)
	{
		
		
		Maze aMaze;
		
		entity1.setMaze(aMaze);// let entity know the maze
		entity2.setMaze(aMaze);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		

		//read the maps as per the level variable
		if (level == 1)
		{
			std::ifstream inFile1("mazeA_10x10.map");
			aMaze.readFile(inFile1);
			
			
		}
		else if (level == 2)
		{
			std::ifstream inFile2("mazeB_25x25.map");
			aMaze.readFile(inFile2);
			
		}
		else if (level==3)
		{
			std::ifstream inFile3("mazeC_40x20.map");
			aMaze.readFile(inFile3);
			
		}
		else if(level==-1) //if level is custom
		{
			std::ifstream inFile4(customFileName);
			
			//try {
				aMaze.readFile(inFile4);
			//	throw(customFileName);
			//}
			//this part doesnt seem to be giving an error while reading invalid file names
			//catch (string customFileName)
			//{
			//	cout << "Entered file name is invalid" << endl;
			//	main();
			//}

		}
		aMaze.paint();
		int noOfEntities = size(aMaze.startVec);
		for (int i = 0; i < noOfEntities; i++)
		{
			Entity entityi;
			entityi.setMaze(aMaze);
			entityi.entity_row = stoi(aMaze.startVec[i][0]);
			entityi.entity_col = stoi(aMaze.startVec[i][1]);
			entityi.goal_row = stoi(aMaze.startVec[i][0]);
			entityi.goal_col = stoi(aMaze.startVec[i][1]);

			entity.push_back(entityi);


		}
		
		
		aMaze.printMaze();
		

		while (FSKEY_NULL == FsInkey())
		{
			//paint maze and entity
			
			aMaze.paint();
			for (int i = 0; i < noOfEntities; i++)
			{
			
				entity[i].drawEntity(20 * (entity[i].entity_col - 1), 20 * (entity[i].entity_row - 1), 1, 0, 0, 125);


			}
			
			glFlush();
			FsPollDevice();
			key = FsInkey();
			mouseEvent = FsGetMouseEvent(leftButton, middleButton,rightButton, locX,locY);
			
			
			if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN )
			{// if left mouse button down the we change the map state
				
				aMaze.changeMapState(locX, locY, mouseEvent);
				aMaze.printMaze();
				aMaze.paint();
			}
			if (key == FSKEY_E || key == FSKEY_S)
			{// if 'E' or 'S' is pressed change map state 
				aMaze.changeMapState(locX, locY, key);
				aMaze.printMaze();
				aMaze.paint();
			}
			if (entity1.reachedGoal())
			{//if goal is reached kill the bug and celebrate
				entity1.killEntity(); //makes kill animation
				glFlush();
				entity1.celebrate(); //Plays music
				if (level <= 2)
				{
					level += 1;
					break;
				}
				else
				{
					cout << "You Win!!!!"<<endl;
					break;
				}
			}
			// take keyboard input and call move function
			if (key == FSKEY_ESC)
			{
				return 0;
			
			}
			else if (key == FSKEY_W) //press 'w' to save file
			{
				saveFile(aMaze);
			}
			else if (key == FSKEY_G) //press 'G' for selfplay
			{
				auto started = std::chrono::high_resolution_clock::now();

				std::vector<std::vector<Cell>> optimalPaths;
				//Map map = readMap("data\\map.txt");
				Map map = readMap("mapTest.txt");
				printMap(map);
				HighLevelSolver solver;
				optimalPaths = solver.solve(map);
				printSolution(optimalPaths, entity);

				auto done = std::chrono::high_resolution_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(done - started).count();

				std::cout << elapsedTime << " milliseconds ";

				for (int i = 0; i < noOfEntities; i++)
				{
					cout << "trace" << i;


					//entity[i].findShortestPath(); //find shortest path using depth first search
					entity[i].selfPlay(); //execute the found path

				}
				
			}
			else
			{
				entity1.move(key); 
			}
			
		}
	}
}