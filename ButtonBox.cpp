#include "ButtonBox.h"

ButtonBox::ButtonBox(SDL_Renderer * renderer, TTF_Font * font, SDL_Color textColor, SDL_Color bgColor, std::string display, std::vector<std::string> options, int i, int j, bool isCentered)
{


	this->textColor = textColor;
	this->bgColor = bgColor;
	this->display = display;

	SDL_Surface * displaySurface = TTF_RenderText_Shaded(font, display.c_str(), textColor, bgColor);
	this->displayTexture = SDL_CreateTextureFromSurface(renderer, displaySurface);
	this->displayWidth = displaySurface->w;
	this->displayHeight = displaySurface->h;
	SDL_FreeSurface(displaySurface);


	int maxButtonHeight = 0;
	int totalButtonWidth = 0;

	for (int i = 0; i < options.size(); i++)
	{
		SDL_Surface * buttonSurface = TTF_RenderText_Shaded(font, options[i].c_str(), textColor, bgColor);
		//if button height is greater than max button height, then update max button height
		maxButtonHeight = (buttonSurface->h > maxButtonHeight) ? buttonSurface->h : maxButtonHeight;
		totalButtonWidth += buttonSurface->w;
		SDL_Surface * buttonInvert = TTF_RenderText_Shaded(font, options[i].c_str(), bgColor, textColor);
		buttonList.push_back(std::unique_ptr<Button>(new Button(renderer, buttonSurface, buttonInvert, 0, 0)));

		SDL_FreeSurface(buttonSurface);
		SDL_FreeSurface(buttonInvert);


	}

	//add a padding variable?
	int padding = 20;
	this->height = (displayHeight + maxButtonHeight) * 3 / 2 + padding * 2;
	this->width = (totalButtonWidth > displayWidth) ? totalButtonWidth : displayWidth;
	this->width += padding * 2;

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

	int buttonY = height - maxButtonHeight - padding + y;
	int space = (width - totalButtonWidth) / (buttonList.size() + 1);
	int buttonX = x + space;

	for (int i = 0; i < buttonList.size(); i++)
	{
		buttonList[i]->x = buttonX;
		buttonX += space + buttonList[i]->getWidth();
		buttonList[i]->y = buttonY;
	}

}

ButtonBox::~ButtonBox()
{
	free();
}

//should this be public? is this really needed?
void ButtonBox::free()
{
	SDL_DestroyTexture(displayTexture);
}

void ButtonBox::updateInput(int otherX, int otherY)
{
	for (int i = 0; i < buttonList.size(); i++)
	{
		if (buttonList[i]->contains(otherX, otherY))
		{
			this->input = i;
			this->validInput = true;
			break;
		}
	}
}

void ButtonBox::updateHover(int otherX, int otherY)
{
	for (int i = 0; i < buttonList.size(); i++)
	{
		if (buttonList[i]->contains(otherX, otherY))
		{
			if (!buttonList[i]->getIsInverted())
			{
				buttonList[i]->toggle();
			}
		}
		else
		{
			if (buttonList[i]->getIsInverted())
			{
				buttonList[i]->toggle();
			}
		}
	}
}

void ButtonBox::render(SDL_Renderer * renderer)
{
	//creating the background of the box
	Uint8 oldR, oldG, oldB, oldA;
	SDL_GetRenderDrawColor(renderer, &oldR, &oldG, &oldB, &oldA);
	SDL_Rect fillRect = {x, y, width, height};
	SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	SDL_RenderFillRect(renderer, &fillRect);

	//note that the + 10 is for the padding TODO: fix this
	SDL_Rect renderQuad = { x + (width - displayWidth)/2, y+10, displayWidth, displayHeight};
	SDL_RenderCopy(renderer, displayTexture, NULL, &renderQuad);

	SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, textColor.a);
	for (int i = 0; i < buttonList.size(); i++)
	{
		int buttonPadding = 2;
		SDL_Rect tempRect = {buttonList[i]->x-buttonPadding, buttonList[i]->y-buttonPadding, buttonList[i]->getWidth() + 2 * buttonPadding, buttonList[i]->getHeight() + 2 * buttonPadding};
		//add a bounding box
		SDL_RenderFillRect(renderer, &tempRect);
		buttonList[i]->render(renderer);
	}


	//creating an outline of the box
	SDL_RenderDrawRect(renderer, &fillRect);
	//reseting renderer color
	SDL_SetRenderDrawColor(renderer, oldR, oldG, oldB, oldA);

}

void ButtonBox::resetInput()
{
	validInput = false;
	input = -1;
}

bool ButtonBox::hasValidInput()
{
	return validInput;
}

int ButtonBox::getInput()
{
	return input;
}

//FOR TESTING ONLY
/*
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#include <cstdio>
#include <iostream>

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
		gWindow = SDL_CreateWindow( "Button Box", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
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

	std::vector<std::string> options;
	options.push_back("Cool.");
	options.push_back("Close Window");
	options.push_back("This button does nothing.");

	ButtonBox mainMenu(gRenderer, myFont, consoleGreen, black, "Welcome to the ButtonBox!", options, SCREEN_WIDTH, SCREEN_HEIGHT, true);

	std::vector<std::string> coolOptions;
	coolOptions.push_back("Ok, go back.");

	ButtonBox cool(gRenderer, myFont, consoleGreen, black, "Cool!", coolOptions, 300, 300, false);

	ButtonBox * current = &mainMenu;

	SDL_Event * e = new SDL_Event();

	bool quit = false;
	bool updateRender = true;
	while (!quit)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				int a, b;
				SDL_GetMouseState(&a, &b);
				current->updateInput(a, b);
				updateRender = true;
			}
			else if (e->type == SDL_MOUSEMOTION)
			{
				int a, b;
				SDL_GetMouseState(&a, &b);
				current->updateHover(a, b);
				updateRender = true;
			}
		}
		if (current->getInput() == 0)
		{
			if (current == &mainMenu)
			{
				current->resetInput();
				current = &cool;
			}
			else
			{
				current->resetInput();
				current = &mainMenu;
			}
		}
		else if (current->getInput() == 1)
		{
			quit = true;
		}
		if (updateRender)
		{
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);
			current->render(gRenderer);
			SDL_RenderPresent(gRenderer);
			updateRender = false;
		}
	}
	delete e;
	ButtonBox * buttons;
	int count = 0;
	while (count++ < 10000)
	{
		ButtonBox * buttons = new ButtonBox(gRenderer, myFont, consoleGreen, black, "Welcome to the ButtonBox!", options, SCREEN_WIDTH, SCREEN_HEIGHT, true);
		SDL_RenderClear(gRenderer);
		buttons->render(gRenderer);
		SDL_RenderPresent(gRenderer);
		delete buttons;
	}
	printf("%d\n", current->getInput());

}
*/
