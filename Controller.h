#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "ButtonBox.h"
#include "Board.h"
#include "Formats.h"
#include "Pattern.h"
#include "Util.h"

enum controlState {menu, running, paused, editing, exiting};

class Controller
{

    Board * board = NULL;

    /*Speed is the frequency of iterations, in iterations/s.
    The system processes this as the period of iterations, or the delay between
    iterations (in ms).*/
    int speed;
    controlState state;

	TTF_Font * mainFont;

	SDL_Color mainColor;
	SDL_Color bgColor;
	SDL_Color accentColor;

	SDL_Renderer * mainRenderer;
	SDL_Event * event;


	SDL_Rect * boardPanel = new SDL_Rect;
	SDL_Rect * statusPanel = new SDL_Rect;

	int currentRow = -1;
	int currentCol = -1;

	void updateRC(int x, int y);
private:
	void renderBoard(SDL_Rect * renderArea);
	void renderStatusPanel(SDL_Rect * renderArea);

	public:
        Controller(SDL_Window * window);
        void createNewBoard(bool wrapAround, int height, int width);
        void createNewBoard(bool wrapAround);
        void createNewBoard(std::string filename);


        void editMode();
		void runningMode();

        void randomizeBoard(double ratio);

        void keybindingsBox();


        int getSpeed();
        controlState getState();
        std::string getStateName();

		void getConfirmationBox(std::string dialog);
		int getButtonInput(std::string dialog, std::vector<std::string> options);
		bool getYesOrNo(std::string dialog);
        std::string getStringInput(std::string message);
		double getRatioInput(std::string message);
		int getIntInput(std::string message);

        void saveCurrent();
        void setState(controlState newState);
        void setSpeed(int newSpeed);

		void updateScreen();
		void renderBoard();
		void renderStatusPanel();

        void renderPattern(std::vector<std::vector<bool>>, int x, int y);
		void addPattern(int startR, int startC);
		
		void runIteration();

		void printPanelDimensions();
};
