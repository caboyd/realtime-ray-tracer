#pragma once
#include "Ray.h"
#include "Vector3.h"
#include "Mat4.h"
#include "Quat.h"

const float SENSITIVITY = 0.005f;

class Camera
{
public:
	Vector3 position;
	Vector3 normal;
	Vector3 target;
	Vector3 screen_horizontal;
	Vector3 screen_vertical;
	Vector3 lower_left_corner;
	float lens_radius;
	float focus_dist;

	float half_width, half_height;
	Vector3 w, u, v;
	float time0, time1;

	float heading,pitch;

	Vector3 world_right, world_up;

	Quat orientation;

	Camera() = default;
	Camera(const Vector3& eye, const Vector3& target, const Vector3& up, float vFov, float aspect_ratio, float t0 = 0.f,
	       float t1 = 0.f);
	Camera(const Vector3& eye, const Vector3& target, const Vector3& up, float vFov, float aspect_ratio, float aperture,
	       float focus_dist, float t0 = 0.f, float t1 = 0.f);

	Ray getRay(float x, float y) const;

	Mat4 getViewMatrix() const;
	Mat4 getViewMatrixInverse() const;

	Vector3 getForward() const;
	Vector3 getRight() const;
	Vector3 getUp() const;

	//Move the coordinate system a distance
	void moveForward(float distance);
	void moveUp(float distance);
	void moveRight(float distance);

	void processMouseMovement(int x_offset, int y_offset, bool constrain_pitch = true);

	void calculateOrientation();
};
