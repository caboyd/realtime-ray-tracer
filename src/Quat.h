#pragma once
#include <ostream>

class Vector3;


//TODO: Unfinished
class Quat
{
public:
	union
	{
		float data[4];
		struct{	float x, y, z, w;};
		struct{ float r, g, b, a;};
	};

	Quat();
	Quat(float x, float y, float z, float w);

	//Copy Constructor
	Quat(const Quat& other);

	//Copy Assignment
	Quat& operator=(const Quat& other);

	//Comparison
	bool operator==(const Quat& other) const;

	void set(float x, float y, float z, float w);

	static Quat fromAxisAngle(const Vector3& axis, float rad);

	static Quat fromIdentity();

	void indentity();

	Quat operator*(const Quat& q) const;

	Quat conjugate() const;
};

//cout operator
std::ostream& operator<<(std::ostream& os, const Quat& q);