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
			controller->clearScreen();
			int height = controller->getIntInput("Enter height: ");
			controller->clearScreen();
			int width = controller->getIntInput("Enter width: ");
			controller->clearScreen();
			bool wrapAround = controller->getYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround, height, width);
			controller->setState(PAUSED);
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
			controller->setState(PAUSED);
			std::cout << "Board loaded.\n";
			break;
		}
		//Load a random board
		case 2:
		{
			controller->clearScreen();
			int height = controller->getIntInput("Enter height: ");
			controller->clearScreen();
			int width = controller->getIntInput("Enter width: ");
			controller->clearScreen();
			bool wrapAround = controller->getYesOrNo("Would you like to enable wrap around?");
			controller->createNewBoard(wrapAround, height, width);
			double ratio = controller->getRatioInput("Enter a ratio");
			//If the user entered nothing, cancel laoding a random board
			if(ratio == -1)
				break;
			controller->randomizeBoard(ratio);
			controller->setState(PAUSED);
			break;
		}
		//Load the pattern editor
		case 3:
		{
			controller->clearScreen();
			int height = controller->getIntInput("Enter height: ");
			controller->clearScreen();
			int width = controller->getIntInput("Enter width: ");
			//If the user entered nothing, cancel loading the pattern editor
			if(height == 0 && width == 0)
				break;
			controller->createNewBoard(false, height, width);
			controller->setState(EDITING);
			controller->renderBoard();
			break;
			std::cout << "coming soon sorry\n";
			break;
		}
		//Exit
		case -1:
		case 4:
			controller->setState(EXITING);
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


	/*
	SDL_Event * eve = new SDL_Event();

	bool doExit = false;

	while (!doExit)
	{
		while(SDL_PollEvent(eve) != 0)
		{
			if (eve->type == SDL_MOUSEWHEEL)
			{
				std::cout << "Event Wheel X: " << eve->wheel.x << std::endl;
				std::cout << "Event Wheel Y: " << eve->wheel.y << std::endl;
				std::cout << "Event Wheel Direction: " << eve->wheel.direction << std::endl;
			}
			else if (eve->type == SDL_MOUSEBUTTONDOWN)
			{
				std::cout << "mouse button down\n";
				if (eve->button.button == SDL_BUTTON_MIDDLE)
				{
					std::cout << eve->button.x << std::endl;
					std::cout << eve->button.y << std::endl;
				}
			}
		}
	}

	if (doExit)
	{
		std::cout << "Exiting." << std::endl;
		exit(0);
	}
	*/
	Controller *controller = new Controller(gWindow);
	std::cout << "controller constructed\n";

	/*
	int count = 0;
	while (count++ < 1000000)
	{
		std::cout << controller->getStringInput("Enter a word: ") << std::endl;
	}
	*/
	controller->printPanelDimensions();
	std::cout << "screen updated\n";
	controller->printPanelDimensions();
	MainMenu(controller);
	std::cout << "main menu completed\n";
	//Main control loop
	while (controller->getState() != EXITING)
	{
		switch (controller->getState())
		{
			case MENU:
				MainMenu(controller);
				break;
			case RUNNING:
				controller->runningMode();
				break;
			case PAUSED:
			case EDITING:
				controller->editMode();
				break;
			case EXITING:
				break;
		}
	}

	//TODO: clean up SDL
    return 0;
}
