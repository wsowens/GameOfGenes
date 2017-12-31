#include <SDL2/SDL.h>
#include <string>

class Button
{
public:
	Button(SDL_Renderer * renderer, SDL_Surface * surface, SDL_Surface * inverted, int x, int y);
	~Button();
	void free();

	void toggle();
	bool contains(int otherX, int otherY);

	void render(SDL_Renderer * renderer, int a, int b);
	void render(SDL_Renderer * renderer);

	int getWidth();
	int getHeight();
	bool getIsInverted();

	//making public = a bad move?
	int x;
	int y;

private:
	int width;
	int height;
	SDL_Texture * primary;
	SDL_Texture * inverted;
	bool isInverted = false;
};
