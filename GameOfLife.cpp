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
			int height = controller->getIntInput("Enter board height: ");
			controller->clearScreen();
			int width = controller->getIntInput("Enter board width: ");
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
				filename = controller->getStringInput("Enter board filename:");
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
			controller->clearScreen();
			double ratio = controller->getRatioInput("Enter the proportion of cells to randomly turn on:");
			controller->createNewBoard(wrapAround, height, width);
			controller->randomizeBoard(ratio);
			controller->setState(PAUSED);
			break;
		}
		//Load the pattern editor
		case 3:
		{
			controller->clearScreen();
			int height = controller->getIntInput("Enter editor height: ");
			controller->clearScreen();
			int width = controller->getIntInput("Enter editor width: ");
			//If the user entered nothing, cancel loading the pattern editor
			if(height == 0 && width == 0)
				break;
			controller->createNewBoard(false, height, width);
			controller->setState(EDITING);
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
	SDL_Renderer * mainRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	if( mainRenderer == NULL )
	{
		std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		throw "Renderer failed to create.";
	}
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0);
	SDL_RenderClear(mainRenderer);
	TTF_Font* myFont = TTF_OpenFont("./assets/consola.ttf", 18);
	SDL_Color mainColor = {0, 0xFF, 0};
	SDL_Color bgColor = {0, 0, 0};
	std::string color[] = {"Red", "Sea Green", "Blue"};
	std::string flavors[] = {"Hot", "Sweet", "Sour", "Superduperhot"};
	std::string ethnic[] = {"Irish", "USA", "Japan"};
	std::vector<std::string> colors(color, color + sizeof(color) / sizeof(std::string));
	std::vector<std::string> flavor(flavors, flavors + sizeof(flavors) / sizeof(std::string));
	std::vector<std::string> ethnics(ethnic, ethnic + sizeof(ethnic) / sizeof(std::string));
	std::vector<std::vector<std::string>> strings;
	strings.push_back(colors);
	strings.push_back(flavor);
	strings.push_back(ethnics);
	std::cerr << "about to make box\n";

	std::string text =
		"GENERAL CONTROLS"
		"\n+, =\tZoom in"
		"\n-\tZoom out"
		"\nArrow Keys\tMove Cursor / Pan Camera"
		"\nScrollwheel \tZoom in / Out"
		"\nRight-Click\tPan Camera"
		"\nR\tReset Zoom"
		"\nH\tShow Help Menu";

	//GridBox * myBox;
	GridBox * myBox;
	SDL_Event * eve = new SDL_Event();

	bool doExit = false;

	while (!doExit)
	{
		while(SDL_PollEvent(eve) != 0)
		{
			if (eve->type == SDL_QUIT)
			{
				doExit = true;
			}
		}
		myBox = new GridBox(mainRenderer, myFont, text, mainColor, bgColor);
		std::cerr << "Dimensions: " << myBox->getWidth() << " , " << myBox->getHeight() << std::endl;
		myBox->render(mainRenderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, true, LEFT);
		//myBox->render(mainRenderer, 0, myBox->getHeight(), true, CENTER);
		//myBox->render(mainRenderer, 0, myBox->getHeight() * 2, false, RIGHT);
		SDL_RenderPresent(mainRenderer);
		delete myBox;
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
	std::cout << "screen updated\n";
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
				controller->pausedMode();
				break;
			case EDITING:
				controller->editorMode();
				break;
			case EXITING:
				break;
		}
	}
	delete controller;
	//TODO: clean up SDL
    return 0;
}
