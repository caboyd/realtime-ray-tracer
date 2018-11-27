#include <iostream>
#include "SDL2/SDL.h"
#include <bitset>

using std::cout;
using std::endl;

Uint32 rgba_to_uint32(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

int main(int argc, char** argv)
{
	std::cout << "test";

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("RealTime Ray-tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600,
	                                      400, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 600, 400);
	Uint32* pixels = new Uint32[600 * 400];

	SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);

	SDL_RenderClear(renderer);

	memset(pixels, 0, sizeof(Uint32) * 600 * 400);

	for (auto i = 0; i < 400; i++)
	{
		for (auto j = 0; j < 600; j++)
		{
			//std::cout <<  i+j << endl;
			const auto r = j * 255 / 600;
			const auto g = i * 255 / 400;
			const auto b = (i + j) * 255 / 1000;;
			const auto a = 255;

			pixels[i * 600 + j] = rgba_to_uint32(r, g, b, a);
		}
	}

	SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * 600);
	SDL_RenderCopy(renderer, texture,NULL,NULL);
	SDL_RenderPresent(renderer);

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

inline Uint32 rgba_to_uint32(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	auto result = r << 24 | g << 16 | b << 8 | a;
	return result;
}
