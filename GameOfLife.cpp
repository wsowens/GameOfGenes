#include "Controller.h"
#include <ctime>
#include <exception>

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800

void MainMenu(Controller *controller)
{
	std::vector<std::string> options;
	options.push_back("Create Empty Board");
	options.push_back("Load a Saved Board");
	options.push_back("Load a Random Board");
	options.push_back("Enter Pattern Editor");
	options.push_back("Exit");

	std::cout << "main menu\n";

	switch( controller->getButtonInput("Welcome to Game of Life (in SDL)", options))
	{
		//Create a new board
		case 0:
		{
			bool wrapAround = controller->getYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			controller->setState(paused);
			break;
		}
		//Load a saved board
		case 1:
		{
			std::string filename = "";
			bool isFileValid = false;
			//Loop until we get a valid filename
			while(!isFileValid)
			{
				filename = controller->getStringInput("Enter a filename:");
				if(filename == "")
					break;
				try
				{
					controller->createNewBoard(filename);
				}
				catch(char const* message)
				{
					controller->getConfirmationBox(message);
					continue;
				}
				isFileValid = true;
			}
			//If user entered nothing, cancel loading a saved board
			if(filename == "")
				break;
			controller->setState(paused);
			std::cout << "Board loaded.\n";
			break;
		}
		//Load a random board
		case 2:
		{
			bool wrapAround = controller->getYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround);
			double ratio = controller->getRatioInput("Enter a ratio");
			//If the user entered nothing, cancel laoding a random board
			if(ratio == -1)
				break;
			controller->randomizeBoard(ratio);
			controller->setState(paused);
			break;
		}
		//Load the pattern editor
		case 3:
		{
			int height = controller->getIntInput("Enter height: ");
			int width = controller->getIntInput("Enter width: ");
			//If the user entered nothing, cancel loading the pattern editor
			if(height == 0 && width == 0)
				break;
			controller->createNewBoard(false, height, width);
			controller->setState(editing);
			controller->renderBoard();
			break;
			std::cout << "coming soon sorry\n";
			break;
		}
		//Exit
		case -1:
		case 4:
			controller->setState(exiting);
			break;
	}

}

SDL_Window * gWindow = NULL;

bool init()
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		std::cout <<  "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			std::cout <<  "Warning: Linear texture filtering not enabled!\n";
		}

		//Create window
		gWindow = SDL_CreateWindow( "Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			std::cout <<  "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			return false;
		}
		else
		{
			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{
				std::cout <<  "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
				return false;
			}
			if (TTF_Init() == -1)
			{
				std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
				return false;
			}

		}
	}
	return true;
}

int main(int argc, char** args)
{
    //Setup
	srand(time(0));
	std::cout << "About to begin\n";
    if (!init() || gWindow == NULL)
	{
		std::cout << "A critical error has occured.";
		return -1;
	}
	else
	{
		std::cout << "initialization complete\n";
	}
    Controller *controller = new Controller(gWindow);
	std::cout << "controller constructed\n";
	controller->printPanelDimensions();
	std::cout << "screen updated\n";
	controller->printPanelDimensions();
	MainMenu(controller);
	std::cout << "main menu completed\n";
	//Main control loop
	while (controller->getState() != exiting)
	{
		switch (controller->getState())
		{
			case menu:
				MainMenu(controller);
				break;
			case running:
				controller->runningMode();
				break;
			case paused:
			case editing:
				controller->editMode();
				break;
			case exiting:
				break;
		}
	}

	//TODO: clean up SDL
    return 0;
}
