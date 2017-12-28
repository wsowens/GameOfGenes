#include "Board.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace std;

//a constructor for the Board class if height, width, and wraparound options are chosen
Board::Board(bool wrap, int h, int w): matrix(h, vector<bool> (w, 0))
{
	this->height = h;
	this->width = w;
	this->wrapAround = wrap;
	this->iterations = 0;
	this->births = 0;
	this->deaths = 0;
	isSaved = true;
	birthRule = {3,};
	survivalRule = {2, 3,};
}

//a constructor for the board class if just a filename is given
Board::Board(string filename)
{
	BoardData data = loadFormat(filename);
	height = data.height;
	width = data.width;
	wrapAround = data.wrapAround;
	iterations = data.iterations;
	births = data.births;
	deaths = data.deaths;
	isSaved = true;
	matrix = data.matrix;
	birthRule = data.birthRule;
	survivalRule = data.survivalRule;
}

void Board::toggle(int r, int c)	//toggles the cell from true to false or false to true
{
	matrix[r][c] = !matrix[r][c];
	isSaved = false;
}

//allows a board to be randomly generated
void Board::randomize(double ratio)
{
	ratio = (ratio < 0) ? 0 : ratio;
	ratio = (ratio > 1) ? 1 : ratio;
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			if (((double)rand()/RAND_MAX) <= ratio)
			{
				toggle(r, c);
			}
		}
	}
}

//counts how many live neighbours a given cell has
int Board::numNeigh(int r,int c)
{
	/*creating a count variable, setting it equal to -1 if cell is alive (thus
	avoiding counting it twice)*/
	int count = ((matrix[r][c]) ? -1 : 0);
	if (wrapAround)
	{
		for (int i = r-1; i <= r+1; i++)
		{
			for (int j = c-1; j <= c+1; j++)
			{
				//std::cout << "r+i " << r+i << "c+j " << c+j <<std::endl;
				if ( matrix[((i < 0) ? height+i : i) % height][((j < 0) ? width + j : j) % width])
					count++;
			}
		}
	}
	/*neighbor count nested for loops for wrapAround = false
	the ternary operators are used to detect when the cell is on the edge of
	the board*/
	else {
		for (int i = ((r == 0) ? 0: r - 1); i <= ((r == height - 1 ) ? r:r+1); i++)
		{
			for (int j = ((c == 0) ? 0: c - 1); j <= ((c == width - 1) ? c:c+1); j++)
			{
				//std::cout << "r+i " << r+i << "c+j " << c+j <<std::endl;	//for testing purposes
				if (matrix[i][j])
					count++;
			}
		}
	}
	//std::cout << "count:" << count <<endl;	//for testing purposes
	return count;
}


//runs one iteration (for example, when the user presses the "Enter" key in the GameOfLife)
void Board::runIteration()
{

	int **nMatrix = new int*[height];	//creates a new matrix

	for (int i = 0; i < height; i++)	//make the second dimension of the matrix
	{
		nMatrix[i] = new int[width];
	}
	for(int r = 0; r < height; r++)	//gets the numvber of live neighbours
	{
		for(int c = 0; c < width; c++)
		{
			nMatrix[r][c] = numNeigh(r, c);
			//std::cout << nMatrix[r][c] << " ";
		}
		//std::cout << endl;
	}
	//std::cout << "Now we are looking at if the cells do what they are supposed to:\n";
	for(int r = 0; r < height; r++)	//runs logic based on whether or not a cell is alive and how many live neighbours it has
	{
		for(int c = 0; c < width; c++)
		{
			if(matrix[r][c] == 1)	//if the cell is alive (equal to 1)
			{
				if(survivalRule.find(nMatrix[r][c]) == survivalRule.end())
				{
					toggle(r, c);
					deaths++;
				}
			}
			else	//if the cell is dead (equal to 0)
			{
				if(birthRule.find(nMatrix[r][c]) != birthRule.end())
				{
					toggle(r, c);
					births++;
				}
			}
		}
	}
	for (int i = 0; i < height; i++)	//delete the matrix that held how many neighbours the cell had (saves space)
	{
		delete nMatrix[i];
	}
	delete nMatrix;
	iterations++;

}

void Board::runIteration(int runs)	//runs the interation the correct number of times
{
	for(int i = 0; i < runs; i++)
	{
		runIteration();
		//std::cout << "This is the matrix after " << i + 1 << " iterations.\n";	//for testing purposes
		//getMatrix();
		//cout << endl;
	}
}

//returns the matrix
vector<vector<bool>>& Board::getMatrix()
{
	return this->matrix;
}

//prints the board as a matrix of 1s and 0s
//very useful for testing purposes
void Board::printBoard()
{
	for(auto row : matrix)
	{
		for(auto cell : row)
			cout << cell << " ";
		cout << endl;
	}
	cout << "Birth Rule: " << set2rule(birthRule) << endl;
	cout << "Survival Rule: " << set2rule(survivalRule) << endl;
	cout << endl;
}

//save a given state or board, given a name for the file
void Board::saveState(string fileName)
{
	ofstream out(fileName);
	out << height << endl;	//first line tells the program the height of the saved matrix
	out << width << endl;	//second line tells the program the width of the saved matrix
	out << wrapAround << endl;	//third line tells the program if wrapAround was true or not
	out << iterations << endl;	//tells the program how many iterations there were
	out << births << endl;	//how many births there were
	out << deaths << endl;	//how many deaths there were
	out << set2rule(birthRule) << endl;  //rule for cells to spawn
	out << set2rule(survivalRule) << endl; //rule for cells to live
	for (int i = 0; i < height; i++)	//tells the program what the matrix actually looked like
	{
		for (int j = 0; j < width; j++)
		{
			out << matrix[i][j];
		}
		out << "\n";
	}
	out.close();
}

//allows the user to add an existing pattern to the board by calling with the filename, along with an x and y position
void Board::addPattern(string fileName, int x, int y)
{
	ifstream in;
	in.open(fileName);

	//check if the file was able to be opened
	if (!in.is_open())
	{
		cerr << "File not opened" << endl;
	}

	int heightOfSaved = 0;
	int widthOfSaved = 0;
	string line;

	heightOfSaved = fs_atoi(in);
	widthOfSaved = fs_atoi(in);

	//initialize new patternMatrix
	bool **patternMatrix = new bool *[heightOfSaved];
	for(int i = 0; i < heightOfSaved; i++)
	{
		patternMatrix[i] = new bool[widthOfSaved];
	}
	std::cout << "Created new pattern matrix" << std::endl;
	//store file values into the patternMatrix
	int row = 0;
	while(getline(in, line))
	{
		for(int i = 0; i <widthOfSaved; i++)
		{
			patternMatrix[row][i] = line.at(i) == '1';
		}
		row++;
	}

	//check if the pattern will fit in the pattern matrix
	if((heightOfSaved + y) > height || (widthOfSaved + x) > width)
	{
		cerr << "Saved Pattern is larger than board" << endl;
	}
	std::cout << "Pattern successfully loaded" << std::endl;
	// used to access the y component of the matrix
	for(int i = 0; i < heightOfSaved; i++)
		for(int j = 0; j < widthOfSaved; j++)
			matrix[x+i][y+j] = patternMatrix[i][j];
	in.close();

}

//allows the user to add an existing pattern to the board by calling with the actual bool matrix, along with an x and y position
void Board::addPattern(vector<vector<bool>> patternMatrix, int y, int x)
{
	for(size_t i = 0; i < patternMatrix.size(); i++)
	{
		for(size_t j = 0; j < patternMatrix[0].size(); j++)
		{
			matrix[(y + i) % matrix.size()][(x + j) % matrix[0].size()] = patternMatrix[i][j];
		}
	}
}

//returns the height of the board
int Board::getHeight()
{
	return height;
}

//returns the width of the board
int Board::getWidth()
{
	return width;
}

//returns the number of iterations that were run
int Board::getIterations()
{
	return iterations;
}

//returns the number of births that occurred
int Board::getBirths()
{
	return this->births;
}

//returns the number of deaths that occurred
int Board::getDeaths()
{
	return deaths;
}

//returns a boolean value of if the board has been saved
bool Board::getIsSaved()
{
	return isSaved;
}

void Board::setBirthRule(set<int> input)
{
	birthRule = input;
}

void Board::setSurvivalRule(set<int> input)
{
	survivalRule = input;
}

string Board::getBirthRule()
{
	return set2rule(birthRule);
}

string Board::getSurvivalRule()
{
	return set2rule(survivalRule);
}

//TODO: add zoom
bool filled = false;
void Board::render(SDL_Renderer * renderer, SDL_Rect * renderArea, SDL_Point * cursor)
{
	SDL_Color consoleGreen = {0, 255, 0};
	SDL_Color highlight = {255, 255, 255};
	SDL_Color background = {0, 0, 0};
	SDL_Color * cellColor;
	int cellWidth = renderArea->w / width;
	int cellHeight = renderArea->h / height;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int x = renderArea->x + cellWidth * j;
			int y = renderArea->y + cellHeight * i;
			SDL_Rect cellRect = {x, y, cellWidth, cellHeight};
			if (!filled)
			{
				cellRect.w = cellWidth - 1;
				cellRect.h = cellWidth - 1;
			}

			cellColor = &consoleGreen;
			if ((cursor->x >= x && cursor->x <= (x + cellWidth-1) ) && (cursor->y >= y && cursor->y <= (y + cellHeight - 1)))
			{
				cellColor = &highlight;
			}
			if (!matrix[i][j])
			{
				cellColor = &background;
			}
			SDL_SetRenderDrawColor(renderer, cellColor->r, cellColor->g, cellColor->b, cellColor->a);
			if (!filled)
			SDL_RenderFillRect(renderer, &cellRect);
		}
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
}

//FOR TESTING ONLY
/*
SDL_Window * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Board Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
			}
		}
	}
	return success;
}

int main(int argc, char** argv)
{

	int height;
	int width;

	cout << "Height of board: ";
	cin >> height;

	cout << "Width of board: ";
	cin >> width;

	if (!init())
	{
		return -1;
	}
	SDL_Event * e = new SDL_Event();
	SDL_RenderClear(gRenderer);
	SDL_RenderPresent(gRenderer);
	bool loop = true;

	while (loop)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return 0;
			}
			else if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				loop = false;
			}
		}
	}
	loop = true;

	SDL_Point pos = {100, 100};
	SDL_Rect rect = {200, 100, 400, 400};
	cout << "Add a board called test\n";
	Board test(false, height, width);

	cout << "Print out the matrix:\n";
	test.printBoard();
	SDL_RenderClear(gRenderer);
	test.render(gRenderer, &rect, &pos);
	SDL_RenderPresent(gRenderer);
	cout << endl;
	while (loop)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return 0;
			}
			else if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				loop = false;
			}
		}
	}
	loop = true;

	cout << "Add a pattern called patterntest.txt and then print out the matrix\n";
	test.addPattern("./boards/_smile.brd", 0, 0);
	test.printBoard();
	SDL_RenderClear(gRenderer);
	test.render(gRenderer, &rect, &pos);
	SDL_RenderPresent(gRenderer);
	cout << endl;
	while (loop)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return 0;
			}
			else if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				loop = false;
			}
		}
	}
	loop = true;

	//test.getMatrix();
	//cout << endl;

	cout << "Try running the runIteration function: \n";
	test.runIteration();

	cout << "This is the matrix now\n";
	SDL_RenderClear(gRenderer);
	test.render(gRenderer, &rect, &pos);
	SDL_RenderPresent(gRenderer);
	test.printBoard();
	cout << endl;
	while (loop)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return 0;
			}
			else if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				loop = false;
			}
		}
	}
	loop = true;

	test.runIteration(3);
	cout << "This is the matrix after 3 runIterations\n";

	SDL_RenderClear(gRenderer);
	test.render(gRenderer, &rect, &pos);
	SDL_RenderPresent(gRenderer);
	test.printBoard();
	cout << endl;


	while (loop)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return 0;
			}
			else if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				loop = false;
			}
		}
	}
	loop = true;

	cout << "Random Board\n";
	test.randomize();
	test.printBoard();
	SDL_RenderClear(gRenderer);
	test.render(gRenderer, &rect, &pos);
	SDL_RenderPresent(gRenderer);
	cout << endl;
	while (loop)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return 0;
			}
			else if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				loop = false;
			}
		}
	}
	loop = true;
	SDL_Rect viewport = {0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2};
	SDL_RenderSetViewport(gRenderer, &viewport);
	SDL_RenderClear(gRenderer);
	cout << "clearning renderer\n";
	SDL_RenderPresent(gRenderer);
	while (loop)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				return 0;
			}
			else if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				loop = false;
			}
		}
	}
	return 0;
}
*/
