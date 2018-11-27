


#include <iostream>
#include "SDL2/SDL.h"

int main( int argc, char **argv )
{
	std::cout << "test";

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("RealTime Ray-tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect rect;
	rect.h = 200, rect.w = 200, rect.x = 200, rect.y = 100;
	SDL_RenderFillRect(renderer, &rect);

	SDL_RenderPresent(renderer);

	SDL_Delay(30000);

	return 0;
}
