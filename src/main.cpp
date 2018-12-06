#include <iostream>
#include "SDL2/SDL.h"
#include "Globals.h"
#include "Random.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Material.h"
#include "Box.h"
#include "PerformanceCounter.h"
#include "Polygon.h"
#include <omp.h>
#include "XYRect.h"
#include "BVHNode.h"
#include "MovingSphere.h"

using std::cout;
using std::endl;

Uint32 rgba_to_uint32(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
Uint32 vector3_to_uint32(const Vector3& color, float alpha = 1);
Vector3 ray_trace(const Ray& ray, Hitable* world, int depth);
Vector3 random_in_unit_sphere();
void generateLightsFromRect(std::vector<Vector3>& lights, float xmin, float xmax, float ymin, float ymax, float zmin,
                            float zmax, float step_size);

Hitable* simpleLight();
Hitable* cornell_box();
Hitable* cornell_gloss();

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
	const float vFOV = 40;
	Vector3 eye(278, 278, 1);
	Vector3 target(278, 278, 0);

	camera = Camera({278, 278, -800}, {278, 278, 0}, {0, 1, 0}, vFOV, ASPECT_RATIO, 0.0, 10.0, 0.0, 1);
	Hitable* world = cornell_box();

	//camera = Camera({5, 2, 10}, {1, 1,-1}, {0, 1, 0}, vFOV, ASPECT_RATIO);
	//Hitable* world = simpleLight();


	PerformanceCounter p{};
	p.start();
	double time = 0;

	int SS = int(sqrtf(MAX_SAMPLES));

#pragma omp parallel for
	for (int y = 0; y < SCREEN_HEIGHT; y++)
	{
		thread_local std::mt19937 gen(std::random_device{}());
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			float u = x, v = y;
			Vector3 color(0, 0, 0);
#ifdef SAMPLING

#ifdef UNIFORM_SAMPLING
			for (int s = 0; s < SS; s++)
			{
				for (int s2 = 0; s2 < SS; s2++)
				{
					u = float(x + float(s) / float(SS) + 1 / float(SS * 2));
					v = float(y + float(s2) / float(SS) + 1 / float(SS * 2));

#else


			for (int s = 0; s < MAX_SAMPLES; s++)
			{
				//Thread safe rand	
				u = Random::randf(gen, x, x + 1);
				v = Random::randf(gen, y, y + 1) ;
#endif
#endif
			u = u / float(SCREEN_WIDTH);
			v = v / float(SCREEN_HEIGHT);

			Ray ray = camera.getRay(u, v);
			//Ray ray = camera.getRay(u, v);
			color += ray_trace(ray, world, 0);

#ifdef SAMPLING
				}

#ifdef UNIFORM_SAMPLING
			}
#endif

			color /= float(MAX_SAMPLES);
#endif

			//color = {sqrtf(color.r), sqrtf(color.g), sqrtf(color.b)};

			//HDR + Gamma Correction Magic
			//https://www.slideshare.net/ozlael/hable-john-uncharted2-hdr-lighting  slide 140
			color -= 0.004;
			color.clampMin(0);
			color = (color * (6.2 * color + 0.5)) / (color * (6.2 * color + 1.7) + 0.06);

			pixels[(SCREEN_HEIGHT - y - 1) * SCREEN_WIDTH + x] = vector3_to_uint32(color);
		}
		//time = p.getCounter() / float(y) / 1000.0f;
		cout << "Row " << y << "/" << SCREEN_HEIGHT << endl;
		//cout << "  Estimated Time remaining:" << (SCREEN_HEIGHT - y) * time << "s" << endl;
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

Vector3 ray_trace(const Ray& ray, Hitable* world, int depth)
{
	HitRecord rec;
	Ray ray_out;
	ray_out.time = ray.time;
	Vector3 attenuation;

	if (world->hit(ray, 0.001f, FLT_MAX, rec))
	{
		Vector3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.position);
		if (depth < MAX_RAY_DEPTH && rec.mat_ptr->scatter(ray, rec, attenuation, ray_out))
		{
#ifdef BAD_SHADOWS

			//blinn-phong lighting
			//no specular yet
			HitRecord last_rec = rec;
			Vector3 color;
			if (last_rec.mat_ptr->shadowsAllowed())
			{
#ifndef PATH_TRACING

				if (dynamic_cast<Dialectric*>(last_rec.mat_ptr))
				{
					color = attenuation * ray_trace(ray_out, world, depth + 1);
					if (last_rec.mat_ptr->reflection(ray, last_rec, attenuation, ray_out))
						color += attenuation * ray_trace(ray_out, world, depth + 1);
					return color;
				}

#endif

				Vector3 diffuse(0);
				Vector3 specular(0);
				for (auto&& light : g_lights)
				{
#ifdef  PATH_TRACING
					Vector3 light_position = light.getRandomLightPoint() - last_rec.position;
#else
					Vector3 light_position = light.center - last_rec.position;
#endif
					float distance = light_position.length();
					Vector3 light_dir = light_position.getNormalized();
					float dot = last_rec.normal.dot(light_dir);
					//Offset bias for acne
					Ray ray(last_rec.position + last_rec.normal * 0.01, light_dir, ray_out.time);
					bool in_shadow = (world->hit(ray, 0.001f, light_position.length() * 0.999, rec));

					diffuse += (1.0f - in_shadow) * dot * light.color * light.power / distance;
					specular += (1.0f - in_shadow) * last_rec.mat_ptr->getSpecular(camera.position - last_rec.position,
					                                                               light_position, last_rec.normal,
					                                                               light.color, light.power);
				}
				diffuse /= float(g_lights.size());
				specular /= float(g_lights.size());
				diffuse += AMBIENT_LIGHT;
				diffuse.clamp(0, 1);
				specular.clamp(0, 100);
				color = diffuse + specular;


				if (dynamic_cast<Lambertian*>(last_rec.mat_ptr))
					color = (diffuse + specular + AMBIENT_LIGHT) * attenuation;
				else if (dynamic_cast<BlinnPhong*>(last_rec.mat_ptr) && !dynamic_cast<BlinnPhong*>(last_rec.mat_ptr)
					->
					reflects)
					color *= attenuation;
				else
					color *= attenuation * ray_trace(ray_out, world, depth + 1);

				return emitted + color;
			}
#endif
			return emitted + attenuation * ray_trace(ray_out, world, depth + 1);
		}
		else

			return emitted;
	}
	else
		return AMBIENT_LIGHT;
}

Hitable* cornell_gloss()
{
	Vector3 red_color(.65f, .05f, .05f);
	Vector3 white_color(0.73f);

	Material* white = new Lambertian(new ConstantTexture({0.73f, 0.73f, 0.73f}));
	Material* red = new Lambertian(new ConstantTexture({0.65f, 0.05f, 0.05f}));
	Material* green = new Lambertian(new ConstantTexture({0.12f, 0.45f, 0.15f}));
	Material* blue = new Lambertian(new ConstantTexture({0.12f, 0.15f, 0.56f}));

	Material* light = new DiffuseLight(new ConstantTexture({60, 60, 60}));

	Material* mirror = new Metal({0.95f, 0.95f, 0.95f}, 0.2f);
	Material* perfectMirror = new Metal({1.f, 1.f, 1.f}, 0.f);

#if 0
	Material* redFuzzy = new BlinnPhong({0.65f, 0.05f, 0.05f}, Vector3(1), 32, true, 0.2f);
	Material* greenFuzzy = new BlinnPhong({0.12f, 0.45f, 0.15f}, Vector3(1), 32, true, 0.2f);
	Material* blueFuzzy = new BlinnPhong({0.12f, 0.15f, 0.56f}, Vector3(1), 32, true, 0.2f);
#else
	Material* redFuzzy = new Metal({0.65f, 0.05f, 0.05f}, 0.2f);
	Material* greenFuzzy = new Metal({0.12f, 0.45f, 0.15f}, 0.2f);
	Material* blueFuzzy = new Metal({0.12f, 0.15f, 0.56f}, 0.2f);
#endif

	Hitable** list = new Hitable*[11];
	int i = 0;

	g_lights.emplace_back(Vector3(50, 50, 0), Vector3(0), Vector3(1), Vector3(300));

	list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, red));
	list[i++] = new YZRect(0, 555, 0, 555, 0, green);
	list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	//floor
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));
	//list[i++] = new XYRect(0, 555, 0, 555, 0, mirror);
	list[i++] = new XYRect(0, 70, 0, 70, 0, light);

	list[i++] = new Sphere({168, 388, 278}, 100, white);
	list[i++] = new Sphere({168, 168, 278}, 100, red);
	list[i++] = new Sphere({388, 388, 278}, 100, green);
	list[i++] = new Sphere({388, 168, 278}, 100, blue);

	return new HitableList(list, i);
}

Hitable* simpleLight()
{
	Texture* t = new ConstantTexture({1, 0, 0});
	Texture* checker = new CheckerTexture(new ConstantTexture({.2f, .3f, .1f}), new ConstantTexture({.9f, .9f, .9f}),
	                                      10);
	Hitable** list = new Hitable*[4];

	list[0] = new Sphere({0, -1000, 0}, 1000, new Lambertian(checker));
	list[1] = new Sphere({0, 1, 2}, 3, new Lambertian(t));
	list[2] = new Sphere({0, 7, 0}, 2, new DiffuseLight(new ConstantTexture({4, 4, 4})));
	list[3] = new XYRect(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture({4, 4, 4})));

	g_lights.emplace_back(Vector3(4, 2, -2), Vector3(2, 2, 0), Vector3(1), Vector3(4));

	return new HitableList(list, 4);
}

Hitable* cornell_box()
{
	Vector3 red_color(.65f, .05f, .05f);
	Vector3 white_color(0.73f);
	Vector3 blue_color(.12f, .15f, .56f);

	Material* white = new Lambertian(new ConstantTexture({0.73f, 0.73f, 0.73f}));
	Material* whiteFuzzy = new Metal({0.73f, 0.73f, 0.73f}, 0.5f);
	Material* red = new Lambertian(new ConstantTexture({0.65f, 0.05f, 0.05f}));

	Material* redFuzzy = new Metal({0.65f, 0.05f, 0.05f}, 0.95f);
	Material* green = new Lambertian(new ConstantTexture({0.12f, 0.45f, 0.15f}));
	Material* greenFuzzy = new Metal({0.12f, 0.45f, 0.15f}, 0.95f);
	Material* blue = new Lambertian(new ConstantTexture({0.12f, 0.15f, 0.56f}));
	Material* light = new DiffuseLight(new ConstantTexture({15, 15, 15}));
	Material* light2 = new DiffuseLight(new ConstantTexture({2, 2, 2}));
	Material* checker = new Lambertian(new CheckerTexture(new ConstantTexture({0.12f, 0.45f, 0.15f}),
	                                                      new ConstantTexture({0.73f, 0.73f, 0.73f}), 0.05f));

	Material* mirror = new Metal(white_color, 0.3f);
	Material* metal = new Metal(white_color, 0.3f);
	Material* dialectric = new Dialectric(white_color, 1.5);
	Material* blinn = new BlinnPhong(red_color, Vector3(1), 128, false, 1.f);

	Hitable** list = new Hitable*[11];
	int i = 0;


	g_lights.emplace_back(Vector3((213 + 343) / 2, 524, (227 + 332) / 2), Vector3(342 - 213, 0, 332 - 227), Vector3(1),
	                      Vector3(300));


	list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, red));
	list[i++] = new YZRect(0, 555, 0, 555, 0, green);
	list[i++] = new XZRect(213, 343, 227, 332, 554, light);
	//list[i++] = new XZRect(0, 555, 0, 555, 554, light2);
	list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	//floor
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));
	//list[i++] = new XYRect(0, 555, 0, 555, 0, mirror);

	list[i++] = new Sphere({450, 80, 250}, 75, dialectric);
	list[i++] = new Sphere({450, 80, 250}, -74, dialectric);
#ifdef PATH_TRACING
	list[i++] = new MovingSphere({105, 80, 250}, {105, 160, 250}, 0.f, 1.f, 75, blinn);
#else
	list[i++] = new Sphere({105, 80, 250}, 75, blinn);
#endif
	//list[i++] = new Sphere({450, 80, 250}, -70, dialectric);
	//list[i++] = new Box({278, 70, 450}, {240, 140, 140}, checker);
	list[i++] = new Sphere({278, 20, 278}, 80, metal);

	//return new BVHNode(list, i, 0, 0);
	return new HitableList(list, i);
}
