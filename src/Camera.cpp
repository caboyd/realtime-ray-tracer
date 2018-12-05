#include "Camera.h"
#include <iostream>
#include "Math.h"
#include "Random.h"

constexpr float M_PI = 3.141592653589793238462643383279502884f; /* pi */

//constructor(eye: vec3, target: vec3, up: vec3, vFov: number, aspect: number, aperture: number, focus_dist: number)
Camera::Camera(const Vector3& eye, const Vector3& target, const Vector3& up, float vFov, float aspect_ratio, float t0,
               float t1)
{
	time0 = t0;
	time1 = t1;
	//Convert to radius
	float theta = (vFov * M_PI) / 180.0f;
	float half_height = tan(theta / 2);
	float half_width = aspect_ratio * half_height;

	position = eye;
	this->normal = (target - eye).getNormalized();
	Vector3 target_to_eye = (eye - target).getNormalized();
	Vector3 right = up.cross(target_to_eye).getNormalized();
	Vector3 actual_up = target_to_eye.cross(right);

	lower_left_corner = this->position - half_width * right - half_height * actual_up - target_to_eye;

	screen_horizontal = right * 2 * half_width;
	screen_vertical = actual_up * 2 * half_height;
}

Camera::Camera(const Vector3& eye, const Vector3& target, const Vector3& up, float vFov, float aspect_ratio,
               float aperture, float focus_dist, float t0, float t1)
{
	time0 = t0;
	time1 = t1;
	lens_radius = aperture / 2.f;
	//Convert to radians
	float theta = (vFov * M_PI) / 180.0f;
	float half_height = tan(theta / 2);
	float half_width = aspect_ratio * half_height;

	position = eye;
	this->normal = (target - eye).getNormalized();
	Vector3 target_to_eye = w = (eye - target).getNormalized();
	Vector3 right = u = up.cross(target_to_eye).getNormalized();
	Vector3 actual_up = v = target_to_eye.cross(right);

	lower_left_corner =
		this->position
		- half_width * focus_dist * right
		- half_height * focus_dist * actual_up
		- focus_dist * target_to_eye;

	screen_horizontal = right * 2 * focus_dist * half_width;
	screen_vertical = actual_up * 2 * focus_dist * half_height;
}

// Camera::Camera(const Vector3& eye, const Vector3& target, const Vector3& up, float focal_length_mm, float film_size_mm,
//                float aspect_ratio)
// {
// 	//https://en.wikipedia.org/wiki/Angle_of_view
// 	//Convert to metres and divide by 2
// 	float half_height = film_size_mm / 2000;
// 	float half_width = aspect_ratio * film_size_mm / 2000;
// 	float f = focal_length_mm / 1000;
//
// 	position = eye;
// 	this->normal = (target - eye).getNormalized();
// 	screen_horizontal = Vector3(1, 0, 0) * 2 * half_width;
// 	screen_vertical = Vector3(0, 1, 0) * 2 * half_height;
// 	lower_left_corner = Vector3(-half_width, -half_height, -f);
// }

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

Mat4 Camera::getViewMatrix() const
{
	Vector3 VPN = normal;
	Vector3 VRP = position;
	Vector3 u, v, n;
	orthoNormalVectors(VPN, Vector3(0, 1, 0), u, v, n);

	Mat4 R = Mat4(u.x, u.y, u.z, 0.f,
	              v.x, v.y, v.z, 0.f,
	              n.x, n.y, n.z, 0.f,
	              0.f, 0.f, 0.f, 1.f);

	Mat4 T = Mat4(1.f, 0.f, 0.f, -VRP.x,
	              0.f, 1.f, 0.f, -VRP.y,
	              0.f, 0.f, 1.f, -VRP.z,
	              0.f, 0.f, 0.f, 1.f);

	//4x4 Matrix Multiplication
	return R * T;
}

Mat4 Camera::getViewMatrixInverse() const
{
	Vector3 VPN = normal;
	Vector3 VRP = position;
	Vector3 u, v, n;
	orthoNormalVectors(VPN, Vector3(0, 1, 0), u, v, n);

	Mat4 R_inv = Mat4(u.x, v.x, n.x, 0.f,
	                  u.y, v.y, n.y, 0.f,
	                  u.z, v.z, n.z, 0.f,
	                  0.f, 0.f, 0.f, 1.f);

	Mat4 T_inv = Mat4(1.f, 0.f, 0.f, VRP.x,
	                  0.f, 1.f, 0.f, VRP.y,
	                  0.f, 0.f, 1.f, VRP.z,
	                  0.f, 0.f, 0.f, 1.f);

	//4x4 Matrix Multiplication
	return T_inv * R_inv;
}
