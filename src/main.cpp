#include <iostream>
#include "SDL2/SDL.h"
#include "Globals.h"
#include "Random.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Material.h"
#include "Box.h"
#include "PerformanceCounter.h"
#include <omp.h>

using std::cout;
using std::endl;

Uint32 rgba_to_uint32(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
Uint32 vector3_to_uint32(const Vector3& color, float alpha = 1);
Vector3 ray_trace(const Ray& ray, Hitable* world, int depth);
Vector3 random_in_unit_sphere();


int main(int argc, char** argv)
{
	Random::init();

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
	Camera camera({0, 0, 0}, {0, 0, -1}, {0, 1, 0}, vFOV, ASPECT_RATIO);

	Hitable* list[8];
	list[0] = new Sphere({0, 0, -1}, 0.5, new Lambertian({0.1f, 0.2f, 0.5f}));
	list[1] = new Sphere({0, -100.5f, -1}, 100, new Lambertian({0.8f, 0.8f, 0.0f}));
	list[2] = new Sphere({1, 0, -1}, 0.5f, new Metal({0.8f, 0.6f, 0.2f}, 0.2f));
	list[3] = new Sphere({-1, 0, -1}, 0.5f, new Dialectric(1.5f));
	//list[4] = new Sphere({-1, 0, -1}, -0.48, new Dialectric(1.5f));
	list[4] = new Box({-0.5f, -0.25f, -0.5f}, {0.25f, 0.25f, 0.125f}, new Lambertian({0.4f, 0.4f, 1.0f}));

	//list[5] = new Polygon({{-10,-1,-1.1},{0,-1,-1.1},{0,10,-1.1},{-10,10,-1.1}},Vector3::UNIT_Z_POS, new Lambertian({0.2,0.2,0.2}));

	Hitable* world = new HitableList(list, 5);

	PerformanceCounter p{};
	p.start();

#pragma omp parallel for
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		thread_local std::mt19937 gen(std::random_device{}());
		for (unsigned int x = 0; x < SCREEN_WIDTH; x++)
		{
			Vector3 color(0, 0, 0);
#ifdef SAMPLING
			for (unsigned int s = 0; s < MAX_SAMPLES; s++)
			{
				//Thread safe rand	
				float u = Random::randf(gen, x - 0.5f, x + 0.5f) / float(SCREEN_WIDTH);
				float v = Random::randf(gen, y - 0.5f, y + 0.5f) / float(SCREEN_HEIGHT);
#else
				float u = x / float(SCREEN_WIDTH);
				float v = y / float(SCREEN_HEIGHT);
#endif
				Ray ray = camera.getRay(u, v);
				//Ray ray = camera.getRay(u, v);
				color += ray_trace(ray, world, 0);
#ifdef SAMPLING
			}

			color /= float(MAX_SAMPLES);
#endif
			color = {sqrtf(color.r), sqrtf(color.g), sqrtf(color.b)};
			pixels[(SCREEN_HEIGHT - y - 1) * SCREEN_WIDTH + x] = vector3_to_uint32(color);
		}
		cout << y << " " << omp_get_thread_num() << endl;
	}

	cout << p.getAndReset() / 1000 << "s" << endl;
	
	SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * SCREEN_WIDTH);
	SDL_RenderCopy(renderer, texture,NULL,NULL);
	SDL_RenderPresent(renderer);

	SDL_Event event;
	bool quit = false;
	while
	(
		!
		quit
	)
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

	return
		0;
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

Vector3 ray_trace(const Ray& ray, Hitable* world, int depth)
{
	HitRecord rec;

	if (world->hit(ray, 0.001f, FLT_MAX, rec))
	{
		Ray ray_out;
		Vector3 attenuation;

		if (depth < MAX_RAY_DEPTH && rec.mat_ptr->scatter(ray, rec, attenuation, ray_out))
		{
			return attenuation * ray_trace(ray_out, world, depth + 1);
		}
		else
		{
			return Vector3::ZERO;
		}
	}
	else
	{
		Vector3 unit_direction = ray.direction.getNormalized();
		float t = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - t) * Vector3(1, 1, 1) + t * Vector3(0.5f, 0.7f, 1.0f);
	}
}
