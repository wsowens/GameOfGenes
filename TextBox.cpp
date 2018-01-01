#include "TextBox.h"

TextBox::TextBox(SDL_Renderer * renderer, TTF_Font * font, SDL_Color textColor, SDL_Color bgColor, std::string display, int i, int j, bool isCentered)
{
	this->textColor = textColor;
	this->bgColor = bgColor;
	this->textFont = font;
	SDL_Surface * displaySurface = TTF_RenderText_Shaded(textFont, display.c_str(), textColor, bgColor);
	this->displayTexture = SDL_CreateTextureFromSurface(renderer, displaySurface);
	this->displayWidth = displaySurface->w;
	this->displayHeight = displaySurface->h;
	SDL_FreeSurface(displaySurface);



	//add a padding variable?
	int padding = 20;
	this->height = (displayHeight) * 7 / 4 + padding * 2;

	//TODO: update the width for text input?
	//this->width = (totalButtonWidth > displayWidth) ? totalButtonWidth : displayWidth;
	this->width = displayWidth + padding * 2;

	if (isCentered)
	{
		this->x = (i - width) / 2;
		this->y = (j - height) / 2;
	}
	else
	{
		this->x = i;
		this->y = j;
	}
}

TextBox::~TextBox()
{
	free();
}

void TextBox::free()
{
	if (displayTexture != nullptr)
	{
		SDL_DestroyTexture(displayTexture);
		displayTexture = nullptr;
	}
}

void TextBox::backspace()
{
	if (input.size() > 0)
		input.pop_back();
}

void TextBox::append(std::string toAppend)
{
	input += toAppend;
}

void TextBox::render(SDL_Renderer * renderer)
{
	//saving old coordinates of the box
	Uint8 oldR, oldG, oldB, oldA;
	SDL_GetRenderDrawColor(renderer, &oldR, &oldG, &oldB, &oldA);

	//creating background
	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_Rect fillRect = {x, y, width, height};
	SDL_RenderFillRect(renderer, &fillRect);

	//rendering the display text
	//+10 is for padding, which we may add later
	SDL_Rect renderQuad = {x + (width - displayWidth)/2, y+10, displayWidth, displayHeight };
	SDL_RenderCopy(renderer, displayTexture, NULL, &renderQuad);



	if (input.size() > 0)
	{
		SDL_Surface * inputSurface = TTF_RenderText_Shaded(textFont, input.c_str(), textColor, bgColor);
		SDL_Texture * inputTexture = SDL_CreateTextureFromSurface(renderer, inputSurface);

		SDL_Rect quad = {x+10, y+height-10-inputSurface->h, inputSurface->w, inputSurface->h};
		SDL_Rect clip = {0, 0, inputSurface->w, inputSurface->h};
		if (inputSurface->w > width -(20))
		{
			quad.w = width - (20);
			clip.w = width - (20);
			clip.x = inputSurface->w - (width - (20));
		}
		SDL_RenderCopy(renderer, inputTexture, &clip, &quad);
		SDL_FreeSurface(inputSurface);
		SDL_DestroyTexture(inputTexture);
	}



	//creating an outline of the box
	SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
	SDL_RenderDrawRect(renderer, &fillRect);
	//reseting renderer color
	SDL_SetRenderDrawColor(renderer, oldR, oldG, oldB, oldA);
}


int TextBox::getWidth()
{
	return displayWidth;
}

int TextBox::getHeight()
{
	return displayHeight;
}


std::string TextBox::getInput()
{
	return input;
}

/*
=============================

FOR TESTING PURPOSES

=============================
*/

/*
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#include <cstdio>
#include <iostream>

SDL_Window * gWindow = NULL;
SDL_Renderer * renderer = NULL;

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
		gWindow = SDL_CreateWindow( "Button Box", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			renderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);
			if( renderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
				//Initialize PNG loading
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

int main(int argc, char** args)
{
	if (!init())
	{
		printf("An error has occurred.");
		return -1;
	}
	TTF_Font * myFont = TTF_OpenFont("./assets/consola.ttf", 28);
	if (myFont == NULL)
	{
		printf("Failed to load Roboto! SDL_ttf Error: %s\n", TTF_GetError());
		return -1;
	}
	SDL_Color consoleGreen = {0, 0xFF, 0};
	SDL_Color black = {0, 0, 0};

	TextBox tbox(renderer, myFont, consoleGreen, black, "Enter your favorite stuff:", SCREEN_WIDTH, SCREEN_HEIGHT, true);

	SDL_Event * e = new SDL_Event();
	bool quit = false;
	bool updateRender = true;

	SDL_StartTextInput();
	while (!quit)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e->type == SDL_KEYDOWN)
			{
				if (e->key.keysym.sym == SDLK_BACKSPACE)
				{
					tbox.backspace();
					updateRender = true;
				}
				else if (e->key.keysym.sym == SDLK_RETURN)
				{
					updateRender = true;
				}
				//TODO: allow copy and paste
			}
			else if (e->type == SDL_TEXTINPUT)
			{
				//TODO: handle copy and paste
				tbox.append(e->text.text);
				updateRender = true;
			}
		}
		if (updateRender)
		{
			tbox.render(renderer);
			SDL_RenderPresent(renderer);
			updateRender = false;
		}

	}
	std::cout << tbox.getInput() << std::endl;
	SDL_StopTextInput();
}
*/
