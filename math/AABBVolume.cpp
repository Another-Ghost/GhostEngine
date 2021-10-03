#include "AABBVolume.h"


vec3 AABBVolume::GetCorner(const vec3& pos, const vec3& dir) const
{
	vec3 corner;
	corner.x = dir.x >= 0 ? half_size.x : -half_size.x;
	corner.y = dir.y >= 0 ? half_size.y : -half_size.y;
	corner.z = dir.z >= 0 ? half_size.z : -half_size.z;
	return corner + pos;
}

float AABBVolume::GetSphereVolumeRadius() const 
{
	//return glm::length(half_size);
	return half_size.x;	//? 由于当前aabb的三维half_size为相同的半对角线长度，所以物体不可能超过以半对角线为半径的球
}

