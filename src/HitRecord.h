#pragma once

class Material;

/**
 * Contains information about a ray hit
 * 
 */
class HitRecord
{
public:

	//Default to max float, indicating no object has been hit yet
	float t{};			//where along the ray the intersection happened
	Material *mat_ptr{};		//The material to determine how to scatter incoming light
	Vector3 position{};	//The position in world coordinates of the intersection
	Vector3 normal{};	//The normal of the object at the intersection point
	float u,v; //Texture coords
	HitRecord() = default;
};


