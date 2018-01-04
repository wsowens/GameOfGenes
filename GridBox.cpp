#include "GridBox.h"
#include <iostream>

std::vector<std::vector<std::string> > parseString(std::string str)
{
	std::cout << str << "|\n";
	std::vector<std::vector<std::string>> strings;
	std::string temp = "";
	std::vector<std::string> vect;
	//std::vector<std::string> * vect = new std::vector<std::string>;
	for (std::string::iterator iter = str.begin(); iter != str.end(); iter++)
	{
		if (*iter == '\n')
		{
			vect.push_back(temp);
			temp = "";
			strings.push_back(vect);
			vect.clear();
		}
		else if (*iter == '\t')
		{
			vect.push_back(temp);
			temp = "";
			if (iter + 1 == str.end())
			{
				strings.push_back(vect);
				vect.clear();
			}
		}
		else
		{
			temp += (*iter);
			if (iter + 1 == str.end())
			{
				vect.push_back(temp);
				temp = "";
				strings.push_back(vect);
				vect.clear();
			}
		}

	}
	for (int i = 0; i < strings.size(); i++)
	{
		for (int j = 0; j < strings[i].size(); j++)
			std::cout << strings[i][j] << ", ";
		std::cout << std::endl;
	}
	return strings;
}

GridBox::GridBox(SDL_Renderer * renderer, TTF_Font * font, std::vector<std::vector<std::string>> strings, SDL_Color textColor, SDL_Color bgColor)
{

	std::cerr << "constructor called \n";
	this->textColor = textColor;
	this->bgColor = bgColor;

	//TODO: possibly convert these to data fields
	int ySpacing = 25;
	int xSpacing = 10;
	int padding = 5;


	std::cerr << "about to make a bunch of textures\n";
	int totalHeight = 0;
	for (int i = 0; i < strings.size(); i++)
	{
		std::cerr << "Texture Vect size: " << textures.size() << std::endl;
		int tempWidth = (strings[i].size() - 1) * xSpacing;
		int maxHeight = 0;

		std::vector<SDL_Texture *> textVect;
		std::vector<SDL_Point> dimVect;
		for (int j = 0; j < strings[i].size(); j++)
		{
			std::cerr << i << " , " << j << std::endl;
			SDL_Surface * renderedText = TTF_RenderText_Shaded(font, strings[i][j].c_str(), textColor, bgColor);
			if (renderedText == NULL)
			{
				std::cout << "IT\'S FUCKING NULL\n";
				exit(-1);
			}
			std::cout <<  strings[i][j] << " " << renderedText->w << " , " << renderedText->h  << std::endl;
			textVect.push_back(SDL_CreateTextureFromSurface(renderer, renderedText));
			dimVect.push_back({renderedText->w, renderedText->h});
			std::cerr << "done \n";
			if (renderedText->h > maxHeight)
				maxHeight = renderedText->h;
			if (j < this->colWidths.size())
			{
				if (renderedText->w > this->colWidths[j])
					this->colWidths[j] = renderedText->w;
			}
			else
			{
				this->colWidths.push_back(renderedText->w);
			}
			SDL_FreeSurface(renderedText);
		}
		this->textures.push_back(textVect);
		this->dimensions.push_back(dimVect);
		totalHeight += maxHeight;
	}



	std::cerr << textures.size() << std::endl;
	std::cerr << textures[0].size() << std::endl;
	int maxRowWidth = 0;
	for (int i = 0; i < this->colWidths.size(); i++)
	{
		maxRowWidth += this->colWidths[i];
		this->colWidths[i] += xSpacing;
	}

	this->width = maxRowWidth + xSpacing * (this->colWidths.size()-1) + padding * 2;
	this->height = totalHeight + (textures.size() - 1) * ySpacing + padding * 2;
	this->xDiff = xSpacing;
	this->yDiff = (height - padding * 2) / (textures.size());
}

GridBox::GridBox(SDL_Renderer * renderer, TTF_Font * font, std::string str, SDL_Color textColor, SDL_Color bgColor):GridBox(renderer, font, parseString(str), textColor, bgColor)
{
	//TODO: delete?
}

GridBox::~GridBox()
{
	for (int i = 0; i < textures.size(); i++)
	{
		for (int j = 0; j < textures[i].size(); j++)
		{
			if (textures[i][j] != nullptr)
			{
				SDL_DestroyTexture(textures[i][j]);
				textures[i][j] = nullptr;
			}
		}
	}
}

void GridBox::render(SDL_Renderer * renderer, int xStart, int yStart, bool isCentered, justify just)
{
	Uint8 oldR, oldG, oldB, oldA;
	SDL_GetRenderDrawColor(renderer, &oldR, &oldG, &oldB, &oldA);
	if (isCentered)
	{
		xStart -= width / 2;
		yStart -= height / 2;
	}
	SDL_Rect fillRect = {xStart, yStart, width, height};

	switch (just)
	{
		case CENTER:
			xStart -= xDiff / 2;
			break;
		case RIGHT:
			xStart -= xDiff;
	}


	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderFillRect(renderer, &fillRect);
	SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);

	int padding = 5;
	int y = padding + yStart + yDiff /4;

	for (int i = 0; i < textures.size(); i++)
	{
		int maxHeight = 0;
		int x = xStart + padding;
		for (int j = 0; j < textures[i].size(); j++)
		{
			//TODO... possibly support multiple styles of centering?
			//assumes a left justification, shifts as necessary
			switch (just)
			{
				case CENTER:
					x += (colWidths[j]) / 2;
					break;
				case RIGHT:
					x += colWidths[j];
			}
			SDL_Rect renderArea = {x, y, dimensions[i][j].x, dimensions[i][j].y};
			switch (just)
			{
				case LEFT:
					x += colWidths[j];
					break;
				case CENTER:
					renderArea.x -= dimensions[i][j].x / 2;
					x += (colWidths[j]) / 2;
					break;
				case RIGHT:
					renderArea.x -= dimensions[i][j].x;
			}
			//SDL_Rect gridDot = {x + j * xDiff -1, }
			//SDL_Renderer
			SDL_RenderCopy(renderer, textures[i][j], NULL, &renderArea);
			//TODO: remove if this doesn't work
			if (dimensions[i][j].y > maxHeight)
			{
				maxHeight = dimensions[i][j].y;
			}
		}
		y += yDiff;
	}

	//creating an outline of the box
	SDL_RenderDrawRect(renderer, &fillRect);
	//reseting renderer color
	SDL_SetRenderDrawColor(renderer, oldR, oldG, oldB, oldA);
}

int GridBox::getWidth()
{
	return width;
}

int GridBox::getHeight()
{
	return height;
}
