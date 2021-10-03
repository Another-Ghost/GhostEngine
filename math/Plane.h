#pragma once
#include "BasicDependencies.h"

struct Plane
{
	vec3 normal;
	float distance;	//��normal����ԭ��ľ���

	Plane() {}

	Plane(const vec3& normal, float distance);

	bool SphereInsidePlane(const vec3& pos, float radius) const;	//inside or intersection

	bool PointInsidePlane(const vec3& pos) const;
};

