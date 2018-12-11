#include "Camera.h"
#include <iostream>
#include "Math.h"
#include "Random.h"
#include "Quat.h"

constexpr float M_PI = 3.141592653589793238462643383279502884f; /* pi */

using std::cout;
using std::endl;

//constructor(eye: vec3, target: vec3, up: vec3, vFov: number, aspect: number, aperture: number, focus_dist: number)
Camera::Camera(const Vector3& eye, const Vector3& target, const Vector3& up, float vFov, float aspect_ratio, float t0,
               float t1)
{
	Camera(eye, target, up, vFov, aspect_ratio,
	       0.0f, 10.f, t0, t1);
}

Camera::Camera(const Vector3& eye, const Vector3& target, const Vector3& up, float vFov, float aspect_ratio,
               float aperture, float focus_dist, float t0, float t1)
{
	time0 = t0;
	time1 = t1;
	lens_radius = aperture / 2.f;
	this->focus_dist = focus_dist;
	//Convert to radians
	float theta = (vFov * M_PI) / 180.0f;
	half_height = tan(theta / 2);
	half_width = aspect_ratio * half_height;

	position = eye;
	this->normal = (target - eye).getNormalized();

	this->pitch = -asin(this->normal.y);
	this->heading = -(atan2(normal.x, normal.z));
	world_right = Vector3(1, 0, 0);
	world_up = Vector3(0, 1, 0);

	this->orientation = Quat::fromIdentity();

	calculateOrientation();
}


Ray Camera::getRay(float x, float y) const
{
	Vector3 rd = lens_radius * Random::random_in_unit_disk();
	Vector3 offset = u * rd.x + v * rd.y;

	float time = Random::randf(time0, time1);

	Vector3 origin = this->position + offset;

	//This is essentially 
	//x = (xmax – xmin)*j/(M – 1) + xmin
	//y = (ymax – ymin)*i/(N – 1) + ymin
	Vector3 direction = this->lower_left_corner + this->screen_horizontal * x + this->screen_vertical * y - origin -
		offset;

	return Ray(origin, direction, time);
}

Vector3 Camera::getForward() const
{
	Quat q = orientation.conjugate();
	Vector3 result(0, 0, -1);
	result.transformQuat(q);
	return result;
}

Vector3 Camera::getRight() const
{
	Quat q = orientation.conjugate();
	Vector3 result(1, 0, 0);
	result.transformQuat(q);
	return result;
}

Vector3 Camera::getUp() const
{
	Quat q = orientation.conjugate();
	Vector3 result(0, 1, 0);
	result.transformQuat(q);
	return result;
}

void Camera::moveForward(float distance)
{
	this->position += this->getForward() * distance;
	this->calculateOrientation();
}

void Camera::moveUp(float distance)
{
	this->position += this->getUp() * distance;
	this->calculateOrientation();
}

void Camera::moveRight(float distance)
{
	this->position += this->getRight() * distance;
	this->calculateOrientation();
}

void Camera::lookAt(const Vector3& target)
{
	normal = target - position;
	normal.normalize();
	this->pitch = -asin(this->normal.y);
	this->heading = -(atan2(normal.x, normal.z));
	this->calculateOrientation();
}

void Camera::processMouseMovement(int x_offset, int y_offset, bool constrain_pitch)

{
	if (x_offset == 0 && y_offset == 0) return;

	this->heading += x_offset * SENSITIVITY;
	if (this->heading > 2 * M_PI) this->heading -= 2 * M_PI;
	if (this->heading < 0) this->heading += 2 * M_PI;

	this->pitch += y_offset * SENSITIVITY;
	if (this->pitch > M_PI) this->pitch -= 2 * M_PI;
	if (this->pitch < -M_PI) this->pitch += 2 * M_PI;

	if (constrain_pitch)
	{
		if (this->pitch > M_PI / 2) this->pitch = M_PI / 2;
		if (this->pitch < -M_PI / 2) this->pitch = -M_PI / 2;
	}

	calculateOrientation();
}

void Camera::calculateOrientation()
{
	Quat pitch_quat = Quat::fromAxisAngle(this->world_right, this->pitch);
	Quat heading_quat = Quat::fromAxisAngle(this->world_up, this->heading);

	this->orientation.indentity();
	this->orientation = this->orientation * pitch_quat;
	this->orientation = this->orientation * heading_quat;

	Vector3 forward = w = getForward();
	Vector3 right = u = getRight();
	Vector3 actual_up = v = getUp();

	lower_left_corner =
		this->position
		- half_width * focus_dist * right
		- half_height * focus_dist * actual_up
		- focus_dist * forward;

	screen_horizontal = right * 2 * focus_dist * half_width;
	screen_vertical = actual_up * 2 * focus_dist * half_height;
}
