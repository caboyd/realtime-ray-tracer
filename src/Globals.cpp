#include "Globals.h"
#include "Vector3.h"
#include "Camera.h"

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const float ASPECT_RATIO = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);


const int MAX_RAY_DEPTH = 50;
const int MAX_SAMPLES = 500;

const Vector3 LIGHT_DIR = Vector3(1.0f, 1.0f, 0.0f).getNormalized();
const Vector3 LIGHT_POS = Vector3(0,4.9f,0.f);
const Vector3 LIGHT_POWER = Vector3(1);
const Vector3 AMBIENT_LIGHT = Vector3(0.5f,0.7f,1.0f);

std::vector<Vector3> g_lights = {};