#include "Globals.h"
#include "Vector3.h"
#include "Camera.h"

const unsigned int SCREEN_WIDTH = 600;
const unsigned int SCREEN_HEIGHT = 400;
const float ASPECT_RATIO = SCREEN_WIDTH / SCREEN_HEIGHT;
const float vFOV = 10;

const unsigned int MAX_RAY_DEPTH = 8;
const unsigned int MAX_SAMPLES = 100;

Vector3 AMBIENT_LIGHT = {0.5f, 0.7f, 1.0f};
