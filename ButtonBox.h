#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "Button.h"
#include <memory>

class ButtonBox
{
public:
	ButtonBox(SDL_Renderer * renderer, TTF_Font * font, SDL_Color textColor, SDL_Color bgColor, std::string display, std::vector<std::string> options, int i, int j, bool isCentered);
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
	SDL_Color textColor;
	SDL_Color bgColor;

	std::string display; //Do we really need this?
	SDL_Texture * displayTexture;
	int displayWidth;
	int displayHeight;

	//TODO: upgrade this to a vector after creating the other constructors for Button
	std::vector<std::unique_ptr<Button>> buttonList;

	bool validInput = false;
	int input = -1;

	int x;
	int y;

	int width;
	int height;
};
