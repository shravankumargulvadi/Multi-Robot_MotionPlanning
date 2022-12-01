#include "Maze.h"

void Maze::readFile(std::ifstream& input)
{
	string wholeLineString; // used to read file one whole line at a time 
	stringstream lineStream; // used to easily get numbers from part of line
	stringstream checkStream;
	int colonLocation; // used to store location of colon in line 
	currRow = 0;
	
	
	while (!input.eof()) {
		lineStream.clear();// just in case 
		int col = 0;
		getline(input, wholeLineString); // read the whole line into a string
		// if there is a colon, set up lineStream to start just after colon 
		// also, remember colonLocation, just in case 
		if ((colonLocation = wholeLineString.find(":")) != string::npos)
		{
			lineStream.str(wholeLineString.substr(colonLocation + 1));
			//std::cout << lineStream.str();
			if (wholeLineString.find("Start:") != string::npos) // find() returns npos when not found
			{
				lineStream >> startRow >> startCol;
				vector<string> newVec;
				newVec.push_back(startRow);
				newVec.push_back(startCol);
				startVec.push_back(newVec);

			}

			else if (wholeLineString.find("End:") != string::npos) // find() returns npos when not found 
			{
				lineStream >> goalRow >> goalCol;
				vector<string> newVec;
				newVec.push_back(goalRow);
				newVec.push_back(goalCol);
				goalVec.push_back(newVec);

			}
			else if (wholeLineString.find("Begin:") != string::npos)
			{
				lineStream >> totalCols;

			}

		}
		

		else 
		{
			string temp;
			checkStream.str(wholeLineString);
			//if we have garbage value in the maze file ignore it
			checkStream >> temp;
			if (temp[0] != '1' && temp[0] != '0')// anything not 1 and 0 is garbage
			{
				continue;
			}
			
			else
			{
				colonLocation = -1;
				lineStream.str(wholeLineString);

				
				while (lineStream >> temp)
				{
					map[currRow][col] = temp;
					col += 1;
				}
				currRow += 1;
			}
			
		}
	}
	maxRow = currRow;// save max row
	mapToPlanner();
}

void Maze::printMaze()
{// print stored data
	cout << "Start: " << startRow <<" "<< startCol << endl;
	cout << "End: " << goalRow << " " << goalCol << endl;
	cout << "Map Begin: " << totalCols << endl;
	
	for (int i = 0; i < currRow; ++i)// loop through rows and columns and print maze
	{
		for (int j = 0; j < stoi(totalCols); ++j)
		{
			std::cout << map[i][j] << ' ';
		}
		std::cout << std::endl;
	}
	cout << "Map End:"<<endl;

}

void Maze::makeQuad(int vert_x,int vert_y,int r,int g,int b)
{// make a 20x20 quad
	glColor3d(r,g,b);
	glBegin(GL_QUADS); 
		glVertex2f(vert_x, vert_y); 
		glVertex2f(vert_x+20.0, vert_y); 
		glVertex2f(vert_x + 20.0, vert_y+20.0);
		glVertex2f(vert_x, vert_y+20.0); 
	glEnd();

}


void Maze::printMessage()
{// print "Kill Bug" for context
	glColor3ub(0, 120,0); 
	glBegin(GL_QUADS);
	glVertex2i(300, 500);
	glVertex2i(300, 800);
	glVertex2i(800, 800);
	glVertex2i(800, 500);
	glEnd();
	glColor3ub(0, 0, 255);
	glRasterPos2i(400, 550); 
	YsGlDrawFontBitmap20x32("Kill the Bug !!");
	glRasterPos2i(400, 600);
	YsGlDrawFontBitmap20x32("Press 'R' to reset");
	glRasterPos2i(350, 650);
	YsGlDrawFontBitmap20x32("Press 'W' to save maze");
	glRasterPos2i(330, 700);
	YsGlDrawFontBitmap20x32("Press 'G' for Self-Play");
	

	
}



void Maze::paint()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	int vert_x = 0, vert_y = 0;
	
	
	
	for (int i = 0; i < 40; ++i)
	{
		for (int j = 0; j < 40;++j)
		{
			if (map[i][j] =="1")
			{// if we have '1' in maze it is an obstacle
				makeQuad(vert_x, vert_y,0,0,0);
				
			}
			else 
			{
				if (i>=maxRow || j>=stoi(totalCols))
				{//beyond the maze size make blue quads (I am color blind, 
				// blue is easier to identify)
					makeQuad(vert_x, vert_y, 255,255,255);
					makeQuad(vert_x, vert_y, 0, 140, 140);

				}
			}

			vert_x += 20;
			
		}
		vert_y += 20;
		vert_x = 0;

		
	}
	//print start and goal
	for (int i = 0; i < size(startVec); i++)
	{
		string stRow = startVec[i][0];
		string stCol = startVec[i][1];
		string glRow = goalVec[i][0];
		string glCol = goalVec[i][1];
		makeQuad(20 * (stoi(stCol) - 1), 20 * (stoi(stRow) - 1), 0, 255, 0);
		makeQuad(20 * (stoi(glCol) - 1), 20 * (stoi(glRow) - 1), 255, 0, 0);
		printMessage();
	}


}

void Maze::changeMapState(int mouseX, int mouseY, int key)
{//take in mouse co-ordinates and change the state of the map at that location
// mouseX and mouseY are the pixel coordinates of a left click 
// key is either FSMOUSEEVENT_LBUTTONDOWN, FSKEY_S, or FSKEY_E 
// mouse click toggles obstacle state of cell/block below mouse location 
// unless block is start or end (start and end MUST be navigable 
// if block is navigable, S and E will set it to either start or end 
// note, start or end may become undefined (-1, -1)
	
	int selectedRow = mouseY / quadSize, selectedCol=mouseX / quadSize; //divide by quad size to get row and col
	if (selectedRow >= 0 && selectedCol >= 0 && selectedRow <= maxRow && selectedCol <= stoi(totalCols))
	{// if row and column are within bounds of the map do the following
		
		if (key == FSMOUSEEVENT_LBUTTONDOWN)
		{
			
			if ((selectedRow != stoi(startRow) && selectedCol != stoi(startCol)) || (selectedRow != stoi(goalRow) && selectedCol != stoi(goalCol)))
			{// dont change if the location is start or goal
			
				map[selectedRow][selectedCol] = to_string(abs(stoi(map[selectedRow][selectedCol]) - 1));

			}
		}
		else if (key == FSKEY_S  && map[selectedRow][selectedCol] != "1")
		{
			
			startRow = to_string(selectedRow+1);
			startCol = to_string(selectedCol+1);
		}
		else if (key == FSKEY_E && map[selectedRow][selectedCol] != "1")
		{
			goalRow = to_string(selectedRow + 1);
			goalCol = to_string(selectedCol + 1);
		}

	}

}


std::ostream& Maze::fileDump(std::ostream& os)
{
	os << maxRow << " " << totalCols << endl;

	for (int i = 0; i <= maxRow; i++) {
		for (int j = 0; j <= stoi(totalCols); j++)
			if (map[i][j] == "1")
			{
				int encoding = i * maxRow + j;
				os << encoding << " ";

			}

	}
	os << endl;
	for (int i = 0; i < size(startVec); i++)
	{
		int start = (stoi(startVec[i][0]) - 1) * maxRow + (stoi(startVec[i][1]) - 1);
		int end = (stoi(goalVec[i][0]) - 1) * maxRow + (stoi(goalVec[i][1]) - 1);
		os << start << " " << end << endl;
	}

	return os;
}




void Maze::mapToPlanner()
{// function to savefile,
	string fileToSave="mapTest.txt";
	//cout << "Enter the name of the file to save >> ";
	//cin >> fileToSave;
	if (fileToSave.length() > 0)
	{
		ofstream outfile(fileToSave);
		if (outfile.is_open())
		{
			fileDump(outfile);
			outfile.close();
		}
		else 
		{
			cout << "could not open the file";
		}
	}
}



