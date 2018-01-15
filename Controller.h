#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "ButtonBox.h"
#include "TextBox.h"
#include "GridBox.h"
#include "Board.h"
#include "Formats.h"
#include "Pattern.h"
#include "Util.h"

enum controlState {MENU, RUNNING, PAUSED, PLACE, EDITING, EXITING};

//TODO: IMPLEMENT
/*
enum statusWords {SIZE, STATUS, ITERATIONS, BIRTHS, DEATHS, SPEED, STATUS_TOTAL};

class WordTexture
{
	WordTexture(SDL_Renderer * renderer, std::string);
	~WordTexture();
	void render(SDL_Renderer * renderer, int x, int y);
	int getWidth();
	int getHeight();

	void free();

private:
	int width;
	int height;
	SDL_Texture * texture;
}
*/

class Controller
{

    Board * board = NULL;

    /*Speed is the frequency of iterations, in iterations/s.
    The system processes this as the period of iterations, or the delay between
    iterations (in ms).*/
    int speed;
    controlState state;

	TTF_Font * mainFont;
	SDL_Renderer * mainRenderer;
	SDL_Texture** statusPanelTextures;

	SDL_Event event;

	SDL_Rect boardPanel;
	SDL_Rect statusPanel;

	SDL_Color mainColor;
	SDL_Color bgColor;
	SDL_Color accentColor;

	SDL_Point boardPosition;

	int currentRow = 0;
	int currentCol = 0;

	int cellWidth = 2;
	int cellHeight = 2;

	GridBox * generalControls = nullptr;
	GridBox * pressAnyKey = nullptr;
	GridBox * pausedControls = nullptr;
	GridBox * runningControls = nullptr;
	GridBox * placeControls = nullptr;
	GridBox * editControls = nullptr;

	private:
		void updateRC(int x, int y);
		void checkRC();
		void renderBoard(SDL_Rect * renderArea);
		void renderStatusPanel(SDL_Rect * renderArea);
        void renderPattern(std::vector<std::vector<bool>>& matrix, SDL_Rect * renderArea);

	public:
		//constructors and destructors
		Controller(SDL_Window * window);
		~Controller();

		//create a new board for this->board
        void createNewBoard(bool wrapAround, int height, int width);
        void createNewBoard(bool wrapAround);
        void createNewBoard(std::string filename);

		//board manipulation methods
        void randomizeBoard(double ratio);
		void runIteration();

		//ACCESSOR METHODS
        int getSpeed();
        controlState getState();
        std::string getStateName();

		//USER INTERFACE METHODS
		//present users with buttons
		int getButtonInput(std::string dialog, std::vector<std::string> options);
		bool getYesOrNo(std::string dialog);
		void getConfirmationBox(std::string dialog);
		void getKeybindingsBox();
		//get users to input text
        std::string getStringInput(std::string message);
		double getRatioInput(std::string message);
		int getIntInput(std::string message);
		void saveCurrent();

		//MUTATOR METHODS
		//control the state / speed of the controller
        void setState(controlState newState);
        void setSpeed(int newSpeed);
		//control the "camera"
		void setPan(int x, int y);
		void setZoom(int amount);
		void resetZoom();

		//RENDERING METHODS
		void updateScreen();
		void clearScreen();
		void renderBoard();
		//instead of keeping track of the old cursor, we can just make a method
		void derenderCursor();
		void renderCursor();
		void renderStatusPanel();

		//CONTROL LOOP METHODS
		void pausedMode();
		void runningMode();
		void placeMode(Pattern pattern);
		void editorMode();

};
