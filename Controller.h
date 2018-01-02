#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "ButtonBox.h"
#include "TextBox.h"
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

	void updateRC(int x, int y);
	void checkRC();

	private:
		void renderBoard(SDL_Rect * renderArea);
		void renderStatusPanel(SDL_Rect * renderArea);

	public:
        Controller(SDL_Window * window);
		~Controller();
        void createNewBoard(bool wrapAround, int height, int width);
        void createNewBoard(bool wrapAround);
        void createNewBoard(std::string filename);


        void editMode();
		void runningMode();
		void placeMode(Pattern pattern);
		void patternMode();

        void randomizeBoard(double ratio);

        void keybindingsBox();


        int getSpeed();
        controlState getState();
        std::string getStateName();

		int getButtonInput(std::string dialog, std::vector<std::string> options);
		bool getYesOrNo(std::string dialog);
		void getConfirmationBox(std::string dialog);
        std::string getStringInput(std::string message);
		double getRatioInput(std::string message);
		int getIntInput(std::string message);

        void saveCurrent();
        void setState(controlState newState);
        void setSpeed(int newSpeed);
		void setPan(int x, int y);
		void setZoom(int amount);
		void resetZoom();

		void updateScreen();
		void clearScreen();
		void renderBoard();
		void renderStatusPanel();

        void renderPattern(std::vector<std::vector<bool>>& matrix, SDL_Rect * renderArea);
		void addPattern(int startR, int startC);

		void runIteration();

		void printPanelDimensions();
};
