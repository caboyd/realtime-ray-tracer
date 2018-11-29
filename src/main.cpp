#include <iostream>
#include "SDL2/SDL.h"
#include "Globals.h"
#include "Random.h"
#include "Sphere.h"
#include "HitableList.h"

using std::cout;
using std::endl;

Uint32 rgba_to_uint32(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
Uint32 vector3_to_uint32(const Vector3& color, float alpha = 1);
Vector3 ray_trace(const Ray& ray, Hitable* world);
Vector3 random_in_unit_sphere();

int main(int argc, char** argv)
{
	std::cout << "test";

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("RealTime Ray-tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                                      SCREEN_WIDTH,
	                                      SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH,
	                                         SCREEN_HEIGHT);
	Uint32* pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	memset(pixels, 0, sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT);

	SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);

	SDL_RenderClear(renderer);

	//Pos, normal, up, vFov, aspect ratio
	Camera camera({0, 0, 0}, {0, 0, -1}, {0, 1, 0}, vFOV, SCREEN_WIDTH / SCREEN_HEIGHT);

	Vector3 llc(-3, -2, -1);
	Vector3 h(6, 0, 0);
	Vector3 vert(0, 4, 0);
	Vector3 origin(0, 0, 0);

	Hitable* list[2];
	list[0] = new Sphere({0, 0, -1}, 0.5);
	list[1] = new Sphere({0, -100.5, -1}, 100);

	Hitable* world = new HitableList(list, 2);

	for (unsigned int y = 0; y < SCREEN_HEIGHT; y++)
	{
		for (unsigned int x = 0; x < SCREEN_WIDTH; x++)
		{
			Vector3 color(0, 0, 0);
			for (unsigned int s = 0; s < MAX_SAMPLES; s++)
			{
				float u = Random::randf(x - 0.5f, x + 0.5f) / float(SCREEN_WIDTH);
				float v = Random::randf(y - 0.5f, y + 0.5f) / float(SCREEN_HEIGHT);

				Ray ray(origin, llc + u * h + v * vert);
				//Ray ray = camera.getRay(u, v);
				color += ray_trace(ray, world);
			}
			if (y == 399)
				int a = 0;
			color /= float(MAX_SAMPLES);
			color = {sqrtf(color.r),sqrtf(color.g),sqrtf(color.b)};
			pixels[(SCREEN_HEIGHT - y - 1) * 600 + x] = vector3_to_uint32(color);
		}
		cout << y << endl;
	}

	SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * 600);
	SDL_RenderCopy(renderer, texture,NULL,NULL);
	SDL_RenderPresent(renderer);

	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		SDL_RenderPresent(renderer);
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

Uint32 vector3_to_uint32(const Vector3& color, float alpha)
{
	int r = int(255.9 * color.r);
	int g = int(255.9 * color.g);
	int b = int(255.9 * color.b);
	int a = int(255.9 * alpha);
	auto result = r << 24 | g << 16 | b << 8 | a;
	return result;
}

Vector3 ray_trace(const Ray& ray, Hitable* world)
{
	HitRecord rec;

	if (world->hit(ray, 0.001, FLT_MAX, rec))
	{
		Vector3 target = rec.position + rec.normal + random_in_unit_sphere();
		return 0.5 * ray_trace(Ray(rec.position, target - rec.position), world);
	}
	else
	{
		Vector3 unit_direction = ray.direction.getNormalized();
		float t = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - t) * Vector3(1, 1, 1) + t * Vector3(0.5f, 0.7f, 1.0f);
	}
}

Vector3 random_in_unit_sphere()
{
	Vector3 p;
	do
	{
		p = Vector3(Random::randf(-1, 1), Random::randf(-1, 1), Random::randf(-1, 1));
	}
	while (p.getSquaredLength() >= 1.0);
	return p;
}
