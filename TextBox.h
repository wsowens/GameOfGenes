#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class TextBox
{
public:
    TextBox(SDL_Renderer * renderer, TTF_Font * font, SDL_Color textColor, SDL_Color bgColor, std::string display, int i, int j, bool isCentered);
	TextBox(SDL_Renderer * renderer, TTF_Font * font, SDL_Color textColor, SDL_Color bgColor, SDL_Color accentColor, std::string display, int i, int j, bool isCentered);
	~TextBox();
	void free();
	//TODO: include a character limit?


	void backspace();
	void append(std::string);
	void render(SDL_Renderer * renderer);

	int getWidth();
	int getHeight();
	std::string getInput();

private:
    SDL_Texture * displayTexture;
	TTF_Font * textFont;
	int displayWidth;
	int displayHeight;

	SDL_Texture * wordTexture;
    std::string input;

	SDL_Color textColor;
	SDL_Color bgColor;
	SDL_Color accentColor;

    int width;
    int height;
	int x;
	int y;
};
