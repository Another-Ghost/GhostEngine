#include "Frustum.h"
#include "Camera.h"
#include "AABBVolume.h"

void Frustum::Initialize(Camera* camera)
{
	mat4 view = camera->ViewMatrix();
	mat4 perspect = camera->PerspectiveMatrix();
	mat4 matrix = camera->PerspectiveMatrix() * camera->ViewMatrix();
	vec3 xaxis = vec3(matrix[0][0], matrix[1][0], matrix[2][0]);
	vec3 yaxis = vec3(matrix[0][1], matrix[1][1], matrix[2][1]);
	vec3 zaxis = vec3(matrix[0][2], matrix[1][2], matrix[2][2]);
	vec3 waxis = vec3(matrix[0][3], matrix[1][3], matrix[2][3]);

	// RIGHT
	plane_array[0] = Plane(waxis - xaxis,
		matrix[3][3] - matrix[3][0]);
	// LEFT
	plane_array[1] = Plane(waxis + xaxis,
		(matrix[3][3] + matrix[3][0]));
	// BOTTOM
	plane_array[2] = Plane(waxis + yaxis,
		(matrix[3][3] + matrix[3][1]));
	// TOP
	plane_array[3] = Plane(waxis - yaxis,
		(matrix[3][3] - matrix[3][1]));
	// NEAR
	plane_array[4] = Plane(waxis + zaxis,
		(matrix[3][3] + matrix[3][2]));
	// FAR
	plane_array[5] = Plane(waxis - zaxis,
		(matrix[3][3] - matrix[3][2]));
}

bool Frustum::AABBIntersection(const vec3& pos, const AABBVolume& aabb) const
{
	//bool x = plane_array[4].normal.x >= 0;
	//bool y = plane_array[4].normal.y >= 0;
	//bool z = plane_array[4].normal.z >= 0;

	//> Optimized View Frustum Culling Algorithms for Bounding Boxes
	if (!SphereIntersection(pos, aabb.GetSphereVolumeRadius()))
	{
		return false;
	}

	//vec3 p = aabb.GetCorner(pos, plane_array[4].normal);	
	//vec3 n = aabb.GetCorner(pos, -plane_array[4].normal);	//the p-vertex has a greater signed distance from the plane than the n-vertex
	//if (!PointInsideFrustum(n))
	//{
	//	return false;
	//}

	return true;
}


bool Frustum::SphereIntersection(const vec3& pos, float radius) const
{
    for (int i = 0; i < 6; ++i)
    {
        if (!plane_array[i].SphereInsidePlane(pos, radius))
        {
            return false;
        }
    }
    return true;
}

bool Frustum::PointInsideFrustum(const vec3& pos) const
{
	for (int i = 0; i < 6; ++i)
	{
		if (!plane_array[i].PointInsidePlane(pos))
		{
			return false;
		}
	}
	return true;
}

