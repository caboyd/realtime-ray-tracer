#include "Vector3.h"
#include "Quat.h"
#include <cassert>


const Vector3 Vector3::ZERO(0.0, 0.0, 0.0);
const Vector3 Vector3::ONE(1.0, 1.0, 1.0);
const Vector3 Vector3::UNIT_X_POS(1, 0, 0);
const Vector3 Vector3::UNIT_X_NEG(-1, 0, 0);
const Vector3 Vector3::UNIT_Y_POS(0, 1, 0);
const Vector3 Vector3::UNIT_Y_NEG(0, -1, 0);
const Vector3 Vector3::UNIT_Z_POS(0, 0, 1);
const Vector3 Vector3::UNIT_Z_NEG(0, 0, -1);

Vector3::Vector3(): x(0), y(0), z(0)
{
}

Vector3::Vector3(float value): x(value), y(value), z(value)
{
}

Vector3::Vector3(float x, float y, float z): x(x), y(y), z(z)
{
}

Vector3::Vector3(const Vector3& other) = default;
Vector3& Vector3::operator=(const Vector3& other) = default;

bool Vector3::operator==(const Vector3& other) const
{
	if (x != other.x) return false;
	if (y != other.y) return false;
	if (z != other.z) return false;
	return true;
}

void Vector3::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3::normalize()
{
	if (isZero()) return;

	const float inv_length = 1.0f / this->length();
	x *= inv_length;
	y *= inv_length;
	z *= inv_length;
}

void Vector3::negate()
{
	x = -x;
	y = -y;
	z = -z;
}

void Vector3::clamp(float min, float max)
{
	x = x < min ? min : x > max ? max : x;
	y = y < min ? min : y > max ? max : y;
	z = z < min ? min : z > max ? max : z;
}

void Vector3::clampMax(float max)
{
	x = x > max ? max : x;
	y = y > max ? max : y;
	z = z > max ? max : z;
}

void Vector3::clampMin(float min)
{
	x = x < min ? min : x;
	y = y < min ? min : y;
	z = z < min ? min : z;
}

void Vector3::mix(const Vector3& other, float blend)
{
	x = other.x * blend + x * (1.0f - blend);
	y = other.y * blend + y * (1.0f - blend);
	z = other.z * blend + z * (1.0f - blend);
}

bool Vector3::isZero() const
{
	if (x != 0.0) return false;;
	if (y != 0.0) return false;
	if (z != 0.0) return false;
	return true;
}

float Vector3::length() const
{
	return sqrtf(x * x + y * y + z * z);
}

float Vector3::getSquaredLength() const
{
	return (x * x + y * y + z * z);
}

Vector3 Vector3::getNormalized() const
{
	if (isZero())
		return Vector3();

	const float inv_length = 1.0f / length();
	return {x * inv_length, y * inv_length, z * inv_length};
}

float Vector3::distance(const Vector3& v) const
{
	return (*this - v).length();
}

float Vector3::distanceSquared(const Vector3& v) const
{
	return (*this - v).getSquaredLength();
}

int Vector3::getLargestComponentIndex() const
{
	float a_x = abs(x);
	float a_y = abs(y);
	float a_z = abs(z);

	if (a_x >= a_y && a_x >= a_z) return 0;
	if (a_y >= a_x && a_y >= a_z) return 1;
	if (a_z >= a_y && a_z >= a_x) return 2;
	assert(false);
	return 0;
}

void Vector3::transformQuat(const Quat& q)
{
	Vector3 qvec = Vector3(q.x,q.y,q.z);
	Vector3 uv(q.y * z - q.z * y, q.z * x - q.x * z, q.x * y - q.y * x);

	//Vector3 uv = qvec.cross(*this);
	//Vector3 uuv = qvec.cross(uv);

	Vector3 uuv(q.y * uv.z - q.z * uv.y, q.z * uv.x - q.x * uv.z, q.x * uv.y - q.y * uv.x);

	float w2 = q.w * 2;

	uv *= w2;
	uuv *= 2;

	*this += uv  + uuv;
}

Vector3 Vector3::operator-() const
{
	return {-x, -y, -z};
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return {this->x + v.x, this->y + v.y, this->z + v.z};
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return {this->x - v.x, this->y - v.y, this->z - v.z};
}

Vector3 Vector3::operator*(const Vector3& v) const
{
	return {this->x * v.x, this->y * v.y, this->z * v.z};
}

Vector3 Vector3::operator/(const Vector3& v) const
{
	return {this->x / v.x, this->y / v.y, this->z / v.z};
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(const Vector3& v)
{
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	return *this;
}

Vector3& Vector3::operator/=(const Vector3& v)
{
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	return *this;
}

Vector3 Vector3::operator-(float f) const
{
	return {x - f, y - f, z - f};
}

Vector3 Vector3::operator+(float f) const
{
	return {x + f, y + f, z + f};
}

Vector3 Vector3::operator/(float scalar) const
{
	return {x / scalar, y / scalar, z / scalar};
}

Vector3 Vector3::operator*(float scalar) const
{
	return {x * scalar, y * scalar, z * scalar};
}

Vector3& Vector3::operator+=(float f)
{
	this->x += f;
	this->y += f;
	this->z += f;
	return *this;
}

Vector3& Vector3::operator-=(float f)
{
	this->x -= f;
	this->y -= f;
	this->z -= f;
	return *this;
}

Vector3& Vector3::operator*=(float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;
	return *this;
}

Vector3& Vector3::operator/=(float f)
{
	this->x /= f;
	this->y /= f;
	this->z /= f;
	return *this;
}

float Vector3::operator[](int i) const
{
	return data[i];
}

float& Vector3::operator[](int i)
{
	return data[i];
}

Vector3 Vector3::cross(const Vector3& v) const
{
	return Vector3
	(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}

float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	if (!os.good())
		return os;
	os << std::fixed;
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	os.unsetf(std::ios_base::fixed);
	return os;
}
