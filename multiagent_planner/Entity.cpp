#include "Entity.h"

void Entity::setMaze(Maze& aMaze)
{
	myMaze = &aMaze;

}



void Entity::move(int direction)
{
	// first check if the chosen direction is navigable 
	// if so erase present cell and draw a new one
	if (direction == FSKEY_UP)
	{
		if (isNavigable(entity_row-1,entity_col))
		{
			//cout << "move up";
			erase();
			entity_row = entity_row - 1;
			paint();
		}
		
		
		

	}
	else if (direction == FSKEY_DOWN)
	{
		if (isNavigable(entity_row+1,entity_col))
		{
			//cout << "move down";
			erase();
			entity_row = entity_row + 1;
			paint();
		}

	}
	else if (direction == FSKEY_LEFT)
	{
		if (isNavigable(entity_row,entity_col-1))
		{
			//cout << "move Left";
			erase();
			entity_col = entity_col - 1;
			paint();
		}
		

	}
	else if (direction == FSKEY_RIGHT)
	{
		if (isNavigable(entity_row,entity_col+1))
		{
			//cout << "move right";
			erase();
			entity_col = entity_col + 1;
			paint();
		}

	}

	else if(direction == FSKEY_R)
		{// if user has pressed R-reset
		erase();
		reset();
		}

}

void Entity::erase()
{//erase by painting a white quad
	myMaze->makeQuad(20 * (entity_col - 1), 20 * (entity_row - 1), 255,255,255);
	
}

void Entity::paint()
{// Paint is same as draw
	
	drawEntity(20 * (entity_col - 1), 20 * (entity_row - 1),1,0,0,125);
	myMaze->makeQuad(20 * (stoi(myMaze->startCol) - 1), 20 * (stoi(myMaze->startRow) - 1), 0, 255, 0);


}

bool Entity::reset() // places entity at maze start
{// move the entity back to start
	entity_row = stoi(myMaze->startRow);
	entity_col = stoi(myMaze->startCol);
	drawEntity(20 * (entity_col - 1), 20 * (entity_row - 1), 1, 0, 0, 125);
	
	return true;

}
bool Entity::reachedGoal()
{// check if goal has been reached return true only if we have reached
	if (stoi(myMaze->goalRow) == entity_row && stoi(myMaze->goalCol) == entity_col)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Entity::isNavigable(int row, int col)
{// check is tyhe requested direction is an obstacle return false if it is
	if (myMaze->map[row - 1][col - 1] == "0")
	{
		return true;
	}
	else 
	{
		return false;

	}
}

void Entity::celebrate()
{
	// celebrate with music
	
	YsSoundPlayer player1; //sound card
	YsSoundPlayer::SoundData myWav1;
	

	
	if (YSOK == myWav1.LoadWav("smb_world_clear.wav"))
	{

		// start the player after opening window
		player1.Start();

		
		player1.PlayOneShot(myWav1);
		

		// need something to keep program running until sound is done
		while (YSTRUE == player1.IsPlaying(myWav1)) {
			//player1.KeepPlaying();  // only really needed when using Linux
			

			// just something to keep track of passing time
			auto nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			//cout << std::ctime(&nowTime);
			
			//
			FsSleep(30);
		}

		// common courtesy to close player, although destructor of player 
		// will end player when variable passes "out of scope" at end of main()
		player1.End();
		
	}

}

void Entity::killEntity()
{
	// kill entity by drawing blood spatter patterns
	erase();
	drawEntity(20 * (entity_col - 1), 20 * (entity_row - 1), 2,255,0,0);

}

void Entity::drawEntity(int vert_x,int vert_y,int inflation, int r, int g, int b)
{ //draw a spider by making diagonal and horizontal quads of specific size
 //inflation of two makes blood spatter like patterns
	glColor3d(r,g,b);
	for (int i = 0; i < 20; i += 2)
	{
		for (int j = 0; j < 20; j += 2)
		{
			
			if (i  == j || i == 20-j || i==10 || j ==10)
			{
				glBegin(GL_QUADS);
				glVertex2f(vert_x+i, vert_y+j);
				glVertex2f(vert_x+i + 2.0* inflation, vert_y+j* inflation);
				glVertex2f(vert_x+i + 2.0* inflation, vert_y+j + 2.0* inflation);
				glVertex2f(vert_x+i, vert_y+j + 2.0* inflation);
				glEnd();
			}
			if (i == 10 && j == 10)
			{
				glBegin(GL_QUADS);
				glVertex2f(vert_x + i-4* inflation, vert_y + j-4* inflation);
				glVertex2f(vert_x + i + 4.0* inflation, vert_y + j-4* inflation);
				glVertex2f(vert_x + i + 4.0* inflation, vert_y + j + 4.0* inflation);
				glVertex2f(vert_x + i-4* inflation, vert_y + j + 4.0* inflation);
				glEnd();

			}

		}

	}
	
}


void Entity::drawCrumbs()
{ // draw small quads at the center of the maze square to trace path
	erase();
	glColor3d(0, 255, 0);
	glBegin(GL_QUADS);
	int vert_x = 20 * (entity_col - 1)+10;
	int vert_y = 20 * (entity_row - 1)+10;
	int inflation = 2;
	glVertex2f(vert_x , vert_y );
	glVertex2f(vert_x  +2.0*inflation, vert_y);
	glVertex2f(vert_x  + 2.0 *inflation, vert_y +  2.0*inflation );
	glVertex2f(vert_x , vert_y +  2.0 *inflation);
	glEnd();



}

vector<cell> Entity::getSuccessors(cell currCell, bool visited[][40])
{ // get valid successors for breadth first search
	vector<int> RowDirection = { 1,0,0,-1 }; // assuming a 4 connected grid
	vector<int> ColDirection = { 0,1,-1,0 };// row and column directions
	vector<cell> successors;
	for (int i = 0; i < RowDirection.size(); i++)
	{
		
		int checkRow = currCell.row + RowDirection[i];
		int checkCol = currCell.col + ColDirection[i];
		
		if (checkRow>=0 && checkCol>=0 && checkRow<=myMaze->maxRow && checkCol<=stoi(myMaze->totalCols) && !(visited[checkRow][checkCol]))
		{// check if the scuccessor candidate is out of the map or if it is already visited
		// since all obstacles are marked '1' in the visited array this check incorporates 
		// obstacles as well
			cell succ;
			succ.row = checkRow;
			succ.col = checkCol;
			successors.push_back(succ);


		}

	}
	
	return successors;



}

void Entity::findShortestPath()
{
	queue<cell> processQueue;
	bool visited[40][40];   //[myMaze->maxRow][myMaze->totalCols]
	cell parent[40][40];
	cell startCell;
	// initializze cell at the current location of the entity
	// and not at the start location so that we can call self play at any point
	startCell.row = entity_row-1; 
	startCell.col = entity_col-1;
	cell endCell;
	endCell.row = stoi(myMaze->goalRow)-1;
	endCell.col = stoi(myMaze->goalCol)-1;
	
	
	//initialize visited, setting obstacles as visited 
	// by doing this we can avoid calling isNavigable

	for (int row = 0; row < myMaze->maxRow; row++)
	{
		for (int col = 0; col < stoi(myMaze->totalCols); col++)
		{
			if (myMaze->map[row][col] != "1")
			{
				visited[row][col] = false;
		
			}
			else
			{
				visited[row][col] = true;
				
			}

		}
		cout << endl;
	}

	visited[stoi(myMaze->startRow)][stoi(myMaze->startCol)] = true;
	processQueue.push(startCell);
	cell nodeToExpand = startCell;
	
	while (!((nodeToExpand.row == endCell.row) && (nodeToExpand.col == endCell.col)))
	{// keep doing the following until we expand goal
		if (processQueue.size() == 0)
		{// if we run out of possible paths print the following
			cout << "No Valid Path found!!!" << endl;
			break;
		}
		
		nodeToExpand = processQueue.front();
		
		
		processQueue.pop();
		visited[nodeToExpand.row][nodeToExpand.col] = true;
		vector<cell> successors=getSuccessors(nodeToExpand,visited);
		for (int i = 0; i < successors.size(); i++)
		{// push valid successor to the queue and set its parent to
		// help with back tracking
			
			processQueue.push(successors[i]);
			parent[successors[i].row][successors[i].col] = nodeToExpand;

		}
	
		
		

	}

	//backtrack to get path
	
	cell cellTrack = endCell;
	optimalPath.push_back(cellTrack);

	while (!(cellTrack.row == startCell.row && cellTrack.col == startCell.col))
	{// until start push parent cells to optimalPath vector
		
		optimalPath.push_back(parent[cellTrack.row][cellTrack.col]);
		cellTrack = parent[cellTrack.row][cellTrack.col];
		
	}
	
	

}

//void Entity::selfPlay()
//{
//	cell nextStep = optimalPath.back();
//	while(!(nextStep.row == (stoi(myMaze->goalRow)-1) && nextStep.col == (stoi(myMaze->goalCol)-1)))
//	{// execute optimal path until we reach goal
//		
//		
//		cell nextStep = optimalPath.back();
//	
//		optimalPath.pop_back();
//		glFlush();
//		drawCrumbs();// drawcrumbs to trace path
//		entity_row = nextStep.row + 1;
//		entity_col = nextStep.col + 1;
//		paint();
//		
//		Sleep(100);
//		if (nextStep.row == stoi(myMaze->goalRow) - 1 && nextStep.col == stoi(myMaze->goalCol) - 1)
//		{
//			break;
//			
//		}
//		
//		
//	}
//
//}


void Entity::selfPlay()
{
	vector<int> nextStep = path.back();
	while (!(nextStep[0] == (stoi(myMaze->goalRow) - 1) && nextStep[1] == (stoi(myMaze->goalCol) - 1)))
	{// execute optimal path until we reach goal


		vector<int> nextStep = path.back();

		path.pop_back();
		glFlush();
		drawCrumbs();// drawcrumbs to trace path
		entity_row = nextStep[0] + 1;
		entity_col = nextStep[1] + 1;
		paint();

		Sleep(100);
		if (nextStep[0] == goal_row - 1 && nextStep[1] == goal_col - 1)
		{
			break;

		}


	}

}
