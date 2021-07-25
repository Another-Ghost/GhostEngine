#include "Plane.h"

Plane::Plane(const vec3& normal_, float distance_)
{
	float length = sqrt(dot(normal_, normal_));

	normal = normal_ / length;
	distance = distance_ / length;
}

bool Plane::SphereInsidePlane(const vec3& pos, float radius) const
{
	if (dot(pos, normal) + distance <= -radius)	
	{
		return false;
	}
	return true;
}

bool Plane::PointInsidePlane(const vec3& pos) const
{
	if (dot(pos, normal) + distance <= 0.f)
	{
		return false;
	}
	return true;
}

