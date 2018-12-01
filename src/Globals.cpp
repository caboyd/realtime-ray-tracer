#include "Globals.h"
#include "Vector3.h"
#include "Camera.h"

const unsigned int SCREEN_WIDTH = 1600;
const unsigned int SCREEN_HEIGHT = 900;
const float ASPECT_RATIO = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);
const float vFOV = 110;

const unsigned int MAX_RAY_DEPTH = 50;
const unsigned int MAX_SAMPLES = 50;

Vector3 AMBIENT_LIGHT = {0.5f, 0.7f, 1.0f};
