#include <math.h>
#include "Quat.h"
#include "Vector3.h"


Quat::Quat(): x(0), y(0), z(0), w(0)
{
}

Quat::Quat(float x, float y, float z, float w): x(x), y(y), z(z), w(w)
{
}

Quat::Quat(const Quat& other) = default;
Quat& Quat::operator=(const Quat& other) = default;

bool Quat::operator==(const Quat& other) const
{
	if (x != other.x) return false;
	if (y != other.y) return false;
	if (z != other.z) return false;
	if (w != other.w) return false;
	return true;
}

void Quat::set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quat Quat::fromAxisAngle(const Vector3& axis, float rad)
{
	float r = rad * 0.5f;
	float s = sin(r);
	return Quat(s * axis.x, s * axis.y, s * axis.z, cos(r));
}

Quat Quat::fromIdentity()
{
	return {0, 0, 0, 1};
}

void Quat::indentity()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

Quat Quat::operator*(const Quat& q) const
{
	return Quat(x * q.w + w * q.x + y * q.z - z * q.y,
	            y * q.w + w * q.y + z * q.x - x * q.z,
	            z * q.w + w * q.z + x * q.y - y * q.x,
	            w * q.w - x * q.x - y * q.y - z * q.z);
}

Quat Quat::conjugate() const
{
	return  Quat(-x,-y,-z,w);
}

std::ostream& operator<<(std::ostream& os, const Quat& q)
{
	if (!os.good())
		return os;
	os << std::fixed;
	os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
	os.unsetf(std::ios_base::fixed);
	return os;
}
