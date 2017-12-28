#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "Button.h"

class ButtonBox
{
public:
	ButtonBox(TTF_Font * font, SDL_Color textColor, SDL_Color bgColor, std::string display, std::vector<std::string> options, int i, int j, bool isCentered);
	~ButtonBox();
	void free();

	void updateInput(int otherX, int otherY);
	void updateHover(int otherX, int otherY);
	//void uodateBoth?

	void render(SDL_Renderer * renderer, int a, int b); //possibly unnecessary
	void render(SDL_Renderer * renderer);

	void resetInput();
	bool hasValidInput();
	int getInput();

private:
	TTF_Font * font; //Do i really need this?
	SDL_Color textColor;
	SDL_Color bgColor;

	std::string display; //Do we really need this?
	SDL_Surface * displaySurface;

	std::vector<Button*> buttonList;

	bool validInput = false;
	int input = -1;

	int x;
	int y;

	int width;
	int height;
};
