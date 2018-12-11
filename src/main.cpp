#include <iostream>
#include "SDL2/SDL.h"
#include "Random.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Material.h"
#include "Box.h"
#include "PerformanceCounter.h"
#include "Polygon.h"
#include "XYRect.h"
#include "MovingSphere.h"
#include "Metal.h"
#include "BlinnPhong.h"
#include "Globals.h"

using std::cout;
using std::endl;

Vector3 uint32_to_vector3(Uint32 color);
Uint32 vector3_to_uint32(const Vector3& color, float alpha = 1);
Vector3 ray_trace(const Ray& ray, Hitable* world, int depth);

Hitable* cornell_box();

void setupCornellWalls(Hitable** list, int& i);

Vector3 eye(278, 278, 1);
Vector3 target(278, 278, 0);
const float vFOV = 40;

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

	//RGB 32bit float data to allow for precise sampling and blending of colors
	Vector3* float_pixels = new Vector3[SCREEN_WIDTH * SCREEN_HEIGHT];
	memset(float_pixels, 0, sizeof(Vector3) * SCREEN_WIDTH * SCREEN_HEIGHT);

	//RGBA 8 bit color for displaying
	Uint32* pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	memset(pixels, 0, sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT);

	SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
	SDL_RenderClear(renderer);

	//Setup Camera and world
	camera = Camera(eye, target, {0, 1, 0}, vFOV, ASPECT_RATIO, 0, (eye - target).length() * 2, 0, 1);
	world = cornell_box();

	//Timer for delta time
	PerformanceCounter time{};
	time.start();

	//number of samples completed
	int samples = 0;

	//Render loop
	bool quit = false;
	while (!quit)
	{
		//Blend factor for each sample
		const double blend_factor = 1.0 / double(samples + 1);

		//Parallelize the loop for each row of pixels
#pragma omp parallel for
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			//Thread safe random generator
			thread_local std::mt19937 gen(std::random_device{}());
			for (int x = 0; x < SCREEN_WIDTH; x++)
			{
				const float fx = float(x), fy = float(y);

				//Jiggle the pixel
				float u = Random::randf(gen, fx, fx + 1);
				float v = Random::randf(gen, fy, fy + 1);

				//Get the pixel in 0 to 1 space
				u = u / float(SCREEN_WIDTH);
				v = v / float(SCREEN_HEIGHT);

				Ray ray = camera.getRay(u, v);

				//Ray trace and get the color of the pixel
				Vector3 color = ray_trace(ray, world, 0);

				//Color is stored in high dynamic range
				//Blend the new color with the old color using blend factor
				float_pixels[(SCREEN_HEIGHT - y - 1) * SCREEN_WIDTH + x].mix(color, blend_factor);

				//HDR + Gamma Correction Magic
				//https://www.slideshare.net/ozlael/hable-john-uncharted2-hdr-lighting  slide 140
				color = float_pixels[(SCREEN_HEIGHT - y - 1) * SCREEN_WIDTH + x];
				color -= 0.004f;
				color.clampMin(0);
				color = (color * (6.2f * color + 0.5f)) / (color * (6.2f * color + 1.7f) + 0.06f);

				//Output color is corrected
				pixels[(SCREEN_HEIGHT - y - 1) * SCREEN_WIDTH + x] = vector3_to_uint32(color);
			}
		}
		samples++;
		cout << "Sample " << samples << endl;

		//Ouput to screen
		SDL_UpdateTexture(texture, NULL, pixels, sizeof(Uint32) * SCREEN_WIDTH);
		SDL_RenderCopy(renderer, texture,NULL,NULL);
		SDL_RenderPresent(renderer);


		//Input/Event Update -------------------
		int last_x = mouse_x, last_y = mouse_y;
		float delta_time = time.getAndReset() / 2.f;
		bool moved = false;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEMOTION:
				if (!mouse_down) break;
				SDL_GetMouseState(&mouse_x, &mouse_y);
				camera.processMouseMovement(mouse_x - last_x, mouse_y - last_y);
				last_x = mouse_x, last_y = mouse_y;
				moved = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mouse_x, &mouse_y);
				last_x = mouse_x, last_y = mouse_y;
				mouse_down = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mouse_down = false;
				break;
			}
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_R])
		{
			moved = true;
			camera.position = eye;
			camera.lookAt(target);
		}
		if (state[SDL_SCANCODE_A])
		{
			camera.moveRight(-delta_time);
			moved = true;
		}

		if (state[SDL_SCANCODE_D])
		{
			moved = true;
			camera.moveRight(delta_time);
		}
		if (state[SDL_SCANCODE_W])
		{
			camera.moveForward(-delta_time);
			moved = true;
		}

		if (state[SDL_SCANCODE_S])
		{
			moved = true;
			camera.moveForward(delta_time);
		}
		if (state[SDL_SCANCODE_SPACE])
		{
			moved = true;
			camera.moveUp(delta_time);
		}

		//If camera moved reset the pixel data
		if (moved)
		{
			memset(float_pixels, 0, sizeof(Vector3) * SCREEN_WIDTH * SCREEN_HEIGHT);
			samples = 0;
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


//Converts a rgb float Vector color to Uint32 rgba 8 bit color
Uint32 vector3_to_uint32(const Vector3& color, float alpha)
{
	int r = int(255.9 * color.r);
	r = std::clamp(r, 0, 255);
	int g = int(255.9 * color.g);
	g = std::clamp(g, 0, 255);
	int b = int(255.9 * color.b);
	b = std::clamp(b, 0, 255);
	int a = int(255.9 * alpha);
	a = std::clamp(a, 0, 255);
	auto result = r << 24 | g << 16 | b << 8 | a;
	return result;
}

//Converts Uint32 rgba 8 bit color to a rgb float Vector color
Vector3 uint32_to_vector3(Uint32 color)
{
	Vector3 rgb(
		float(char(color >> 24)) / 255.f, float(char(color >> 16)) / 255.f, float(char(color >> 8)) / 255.f);
	return rgb;
}

Vector3 ray_trace(const Ray& ray, Hitable* world, int depth)
{
	HitRecord rec;
	Ray ray_out;
	ray_out.time = ray.time;
	Vector3 attenuation;

	if (world->hit(ray, 0.001f, FLT_MAX, rec))
	{
		Vector3 emitted = rec.mat_ptr->emitted(ray, rec);
		if (depth < MAX_RAY_DEPTH && rec.mat_ptr->scatter(ray, rec, attenuation, ray_out))
		{
			return emitted + attenuation * ray_trace(ray_out, world, depth + 1);
		}
		else
			return emitted;
	}
	else
		return AMBIENT_LIGHT;
}


void setupCornellWalls(Hitable** list, int& i)
{
	//left walls
	list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, red_gloss));
	//right
	list[i++] = new YZRect(0, 555, 0, 555, 0, green_gloss);
	//ceiling
	list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white_matte));
	//floor
	list[i++] = new XZRect(0, 555, 0, 555, 0, white_matte);
	//back
	list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white_gloss));
	//front
	list[i++] = new XYRect(0, 555, 0, 555, 0, white_gloss);
}

Hitable* cornell_box()
{
	Material* light = new DiffuseLight(new ConstantTexture({15, 15, 15}));
	Material* light2 = new DiffuseLight(new ConstantTexture({2, 2, 2}));


	Material* metal = new Metal(white_color, 0.0f);
	Material* dialectric = new Dialectric(white_color, 2.54);

	Hitable** list = new Hitable*[11];
	int i = 0;

	g_lights.emplace_back(Vector3((150 + 400) / 2, 524, (150 + 400) / 2), Vector3(400 - 150, 0, 400 - 150), Vector3(1),
	                      Vector3(300));

	setupCornellWalls(list, i);
	//list[i++] = new XZRect(150, 400, 150, 400, 554.9, light);
	list[i++] = new XZRect(0, 555, 0, 555, 554, light2);

	list[i++] = new Sphere({450, 80, 250}, 75, dialectric);
#ifdef DISTRIBUTED_RAYS
	list[i++] = new MovingSphere({105, 80, 250}, {105, 160, 250}, 0.f, 1.f, 75, red_matte);
#else
	list[i++] = new Sphere({105, 80, 250}, 75, blinn);
#endif
	list[i++] = new Box({475, 75, 450}, {100, 150, 100}, checker);
	list[i++] = new Sphere({278, 20, 278}, 80, metal);

	return new HitableList(list, i);
}
