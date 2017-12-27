#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

class Button
{
public:
	Button(SDL_Surface * surface, SDL_Surface * inverted, int x, int y);
	~Button();
	void free();

	void toggle();
	bool contains(int otherX, int otherY);

	void render(SDL_Renderer * renderer, int a, int b);
	void render(SDL_Renderer * renderer);

	int getWidth();
	int getHeight();
	bool getIsInverted();
	void setPosition(SDL_Point * pos);

	//making public = a bad move?
	int x;
	int y;

private:
	SDL_Surface * surface;
	SDL_Surface * inverted;
	bool isInverted = false;
};
