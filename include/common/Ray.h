#pragma once
#include "Prerequisite.h"

struct RayIntersectionInfo
{
	vec3 position;
	float distance;
};

struct Ray
{
	vec3 origin;
	vec3 direction;

	bool AABBIntersectionTest(const vec3& aabb_pos, const vec3& half_dimension, RayIntersectionInfo& info);
};

