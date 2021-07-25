#pragma once
#include "BoundingVolume.h"

struct AABBVolume : public BoundingVolume
{

	vec3 half_size;

	AABBVolume(const vec3& half_size_ = vec3{1.f}) :half_size(half_size_) {}

	AABBVolume operator*(const vec3& scale)
	{
		return AABBVolume(half_size * scale);
	}

	AABBVolume operator*(float scale)
	{
		return AABBVolume(half_size * scale);
	}

	//vector<vec3> GetCornerArray(const vec3& pos);

	// dir: xyz
	vec3 GetCorner(const vec3& pos, const vec3& dir) const;

	float GetSphereVolumeRadius() const;

};

