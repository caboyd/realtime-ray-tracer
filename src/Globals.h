#pragma once

#include "Vector3.h"
#include "Camera.h"

#define global_extern extern

#define SAMPLING

global_extern const unsigned int SCREEN_WIDTH;
global_extern const unsigned int SCREEN_HEIGHT;
global_extern const float ASPECT_RATIO;
global_extern const float vFOV;

global_extern const unsigned int MAX_RAY_DEPTH;
global_extern const unsigned int MAX_SAMPLES;

global_extern Vector3 AMBIENT_LIGHT;

//Pos, normal, up, vFov, aspect ratio
global_extern Camera camera;
