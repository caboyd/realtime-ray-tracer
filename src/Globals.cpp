#include "Globals.h"
#include "Vector3.h"
#include "Camera.h"

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;
const float ASPECT_RATIO = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);


const int MAX_RAY_DEPTH = 50;
const int MAX_SAMPLES = 1000;

const Vector3 LIGHT_DIR = Vector3(1.0f, 1.0f, 0.0f).getNormalized();
const Vector3 LIGHT_POS = Vector3(0, 4.9f, 0.f);
const Vector3 LIGHT_POWER = Vector3(1);
const Vector3 AMBIENT_LIGHT = Vector3(0.1f, 0.1f, 0.1f);

Camera camera = Camera();

std::vector<Light> g_lights = {};

const Vector3 red_color = Vector3(.65f, .05f, .05f);
const Vector3 blue_color = Vector3(.12f, .15f, .56f);
const Vector3 green_color = Vector3(.12f, .45f, .15f);
const Vector3 white_color = Vector3(0.73f);
