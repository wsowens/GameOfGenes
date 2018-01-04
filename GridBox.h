#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <vector>
#include <string>

enum justify {LEFT, CENTER, RIGHT};

class GridBox
{
private:
	std::vector<std::vector<SDL_Texture*>> textures;
	std::vector<std::vector<SDL_Point>> dimensions;

	SDL_Color textColor;
	SDL_Color bgColor;

	int width;
	int height;

	std::vector<int> colWidths;
	int xDiff;
	int yDiff;
public:
	GridBox(SDL_Renderer * renderer, TTF_Font * font, std::vector<std::vector<std::string>> strings, SDL_Color textColor, SDL_Color bgColor);
	GridBox(SDL_Renderer * renderer, TTF_Font * font, std::string str, SDL_Color textColor, SDL_Color bgColor);
	~GridBox();

	void render(SDL_Renderer * renderer, int xStart, int yStart, bool isCentered, justify just);

	int getWidth();
	int getHeight();
};
