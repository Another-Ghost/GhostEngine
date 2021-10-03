#pragma once
#include "BasicDependencies.h"

struct Plane
{
	vec3 normal;
	float distance;	//沿normal方向到原点的距离

	Plane() {}

	Plane(const vec3& normal, float distance);

	bool SphereInsidePlane(const vec3& pos, float radius) const;	//inside or intersection

	bool PointInsidePlane(const vec3& pos) const;
};

