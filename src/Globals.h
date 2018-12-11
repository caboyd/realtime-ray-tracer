#pragma once
#include <vector>
#include "Light.h"
class Vector3;
class Camera;
class Material;
class Hitable;

#define global_extern extern

#define PATH_TRACING
#define DISTRIBUTED_RAYS

global_extern int mouse_x,mouse_y;
global_extern bool mouse_down;

global_extern const int SCREEN_WIDTH;
global_extern const int SCREEN_HEIGHT;
global_extern const float ASPECT_RATIO;

global_extern const int MAX_RAY_DEPTH;

global_extern const Vector3 LIGHT_DIR;
global_extern const Vector3 LIGHT_POS;
global_extern const Vector3 LIGHT_POWER;
global_extern const Vector3 AMBIENT_LIGHT;

//Pos, normal, up, vFov, aspect ratio
global_extern Camera camera;
global_extern Hitable* world;

global_extern std::vector<Light> g_lights;

global_extern const Vector3 red_color;
global_extern const Vector3 blue_color;
global_extern const Vector3 green_color;
global_extern const Vector3 white_color;

global_extern Material* white_matte;
global_extern Material* red_matte;
global_extern Material* green_matte;
global_extern Material* blue_matte;

global_extern Material* white_gloss;
global_extern Material* red_gloss;
global_extern Material* green_gloss;
global_extern Material* blue_gloss;

global_extern Material* checker;