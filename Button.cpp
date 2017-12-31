#include "Button.h"

//TODO: Rule of 3?
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

Button::Button(SDL_Renderer * renderer, SDL_Surface * surface, SDL_Surface * inverted,  int x, int y)
{
	this->primary = SDL_CreateTextureFromSurface(renderer, surface);
	//TODO: raise an exception if inverted does not match the dimensions of surface
	this->inverted = SDL_CreateTextureFromSurface(renderer, inverted);
	this->x = x;
	this->y = y;
	this->width = surface->w;
	this->height = surface->h;
}

//TODO: add copy constructors?
Button::~Button()
{
	free();
}

void Button::free()
{
	//Button is not responsible for freeing surfaces! RAII
	SDL_DestroyTexture(primary);
	SDL_DestroyTexture(inverted);
	printf("Button destroyed.\n");
}

void Button::toggle()
{
	isInverted = !isInverted;
}

bool Button::contains(int otherX, int otherY)
{
	return (otherX >= x && otherX <= (x + width) ) && (otherY >= y && otherY <= (y + height) );
}

//remove if you can't find a good reason to be able to render in a different place
void Button::render(SDL_Renderer * renderer, int a, int b)
{
	SDL_Texture * texture = (isInverted) ? inverted : primary;
	SDL_Rect renderQuad = { a, b, width, height};
	SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

void Button::render(SDL_Renderer * renderer)
{
	render(renderer, x, y);
}

int Button::getWidth()
{
	return width;
}

int Button::getHeight()
{
	return height;
}

bool Button::getIsInverted()
{
	return isInverted;
}

/*
=============================

FOR TESTING PURPOSES

=============================
*/





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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
			}
		}
	}

	return success;
}

int main(int argc, char** args)
{
	if (!init())
	{
		return -1;
	}
	if (gRenderer == NULL)
	{
		printf("oh shit\n");
		return 0;
	}
	else
	{
		printf("%d\n", gRenderer);
	}
	SDL_Surface * on = IMG_Load("on.png");
	SDL_Surface * off = IMG_Load("off.png");
	if (on == NULL || off == NULL)
	{
		printf("Error. Image failed to load.");
		return 1;
	}
	Button * button = new Button(gRenderer, on, off, (SCREEN_WIDTH - on->w)/2, (SCREEN_HEIGHT - on->h)/2);
	SDL_Event * e = new SDL_Event();
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(e) != 0)
		{
			if (e->type == SDL_QUIT)
			{
				quit = true;
				delete button;
			}
			else if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				int a, b;
				SDL_GetMouseState(&a, &b);
				if (button->contains(a,b))
					button->toggle();
			}
		}
		button->render(gRenderer);
		SDL_RenderPresent(gRenderer);

		if (button->getIsInverted())
		{
			delete button;
			button = new Button(gRenderer, on, off, (SCREEN_WIDTH - on->w)/2, (SCREEN_HEIGHT - on->h)/2);
		}
	}
}
*/
