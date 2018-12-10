#include "Globals.h"
#include "Vector3.h"
#include "Camera.h"
#include "BlinnPhong.h"

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;
const float ASPECT_RATIO = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);


const int MAX_RAY_DEPTH = 8;

int mouse_x = 0;
int mouse_y = 0;
bool mouse_down = false;

const Vector3 LIGHT_DIR = Vector3(1.0f, 1.0f, 0.0f).getNormalized();
const Vector3 LIGHT_POS = Vector3(0, 4.9f, 0.f);
const Vector3 LIGHT_POWER = Vector3(1);
const Vector3 AMBIENT_LIGHT = Vector3(0.1f, 0.1f, 0.1f);

Camera camera = Camera();
Hitable* world = NULL;

std::vector<Light> g_lights = {};

const Vector3 red_color = Vector3(.65f, .05f, .05f);
const Vector3 blue_color = Vector3(.12f, .15f, .56f);
const Vector3 green_color = Vector3(.12f, .45f, .15f);
const Vector3 white_color = Vector3(0.73f);

#ifdef PATH_TRACING
Material* white_matte = new Lambertian(new ConstantTexture(white_color));
Material* red_matte = new Lambertian(new ConstantTexture(red_color));
Material* green_matte = new Lambertian(new ConstantTexture(green_color));
Material* blue_matte = new Lambertian(new ConstantTexture(blue_color));
#else
Material* white_matte = new BlinnPhong(white_color, white_color);
Material* red_matte = new BlinnPhong(red_color, red_color);
Material* green_matte = new BlinnPhong(green_color, green_color);
Material* blue_matte = new BlinnPhong(blue_color, blue_color);
#endif

bool reflects = true;
float fuzz = 0.01f;
Material* white_gloss = new BlinnPhong(white_color, white_color, 512, reflects, fuzz);
Material* red_gloss = new BlinnPhong(red_color, red_color, 256, reflects, fuzz);
Material* green_gloss = new BlinnPhong(green_color, green_color, 128, reflects, fuzz);
Material* blue_gloss = new BlinnPhong(blue_color, blue_color, 64, reflects, fuzz);