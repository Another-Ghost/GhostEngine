#pragma once
#include "Prerequisite.h"
#include "Plane.h"

struct Frustum
{
	Plane plane_array[6];

	Frustum() {}
	//void InitFromMatrix(const mat4& mvp_mat);
	void Initialize(Camera* camera);

	bool AABBIntersection(const vec3& pos, const AABBVolume& aabb) const;

	bool SphereIntersection(const vec3& pos, float radius) const;

	bool PointInsideFrustum(const vec3& pos) const;



};

