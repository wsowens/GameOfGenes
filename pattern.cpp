/*
 * pattern.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: erikabryant
 */

#include <fstream>
#include <string>
#include "pattern.h"

Pattern::Pattern(std::string filename)
{
	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
	{
		cerr << "File not opened" << endl;
	}
	string line;

	getline(in, line);					//read out the heightOfSaved
	heightOfPattern = atoi(line.c_str());	//make it an int
	getline(in, line);					//read out the widthOfSaved
	widthOfPattern = atoi(line.c_str());	//make it an int

	//initialize new patternMatrix
	patternMatrix = new bool *[heightOfPattern];
	for(int i = 0; i < heightOfPattern; i++)
	{
		patternMatrix[i] = new bool[widthOfPattern];
	}

	//store file values into the patternMatrix
	int row = 0;
	while(getline(in, line))
	{
		for(int i = 0; i < widthOfPattern; i++)
		{
			patternMatrix[row][i] = line.at(i) == '1';
		}
		row++;
	}

	//check if the pattern will fit in the pattern matrix
	if(height < heightOfPattern || width < widthOfPattern)
	{
		cerr << "Saved Pattern is larger than board" << endl;
	}
}

//for square patterns
void Pattern::rotateSquare(int size)
{
	int patternCopy[15][15] = {{0}};
	for(int i =0; i<size; i++){
		for(int j =0; j<size; j++)
		{
            patternCopy [i][j] = patternMatrix[i][j];
        }
    }
}

//for rectangle patterns
void Pattern::rotate(int rot)
{
	int rotateRectangle[width][height] = {{0}};
	for(int i=0; i<height; i++)
	{
		for (int j=0; j<width; j++)
		{
		rotateRectangle[width][height] = patternMatrix[height][width];
		}
	}
}

int Pattern::getPatternHeight()
{
	return this->heightOfPattern;
}

int Pattern::getPatternWidth()
{
	return this->widthOfPattern;
}



