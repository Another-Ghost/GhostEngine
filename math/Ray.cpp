#include "Ray.h"


bool Ray::AABBIntersectionTest(const vec3& aabb_pos, const vec3& half_dimension, RayIntersectionInfo& info)
{

	vec3 aabb_min = aabb_pos - half_dimension;
	vec3 aabb_max = aabb_pos + half_dimension;

	vec3 t_values{ -1.f, -1.f, -1.f };

	for (int i = 0; i < 3; ++i)	//calculate t_exit
	{
		if (direction[i] > 0)
		{
			t_values[i] = (aabb_min[i] - origin[i]) / direction[i];
		}
		else if (direction[i] < 0)
		{
			t_values[i] = (aabb_max[i] - origin[i]) / direction[i];
		}
	}
	float best_t = MathMethod::MaxElement(t_values);	//check if the ray enters all pairs of slabs
	if (best_t < 0.f)
	{
		return false;
	}

	vec3 intersection = origin + (direction * best_t);
	const float epsilon{ 0.00001f };
	for (int i = 0; i < 3; ++i)
	{
		if (intersection[i] + epsilon < aabb_min[i] ||
			intersection[i] - epsilon > aabb_max[i])
		{
			return false;
		}
	}

	info.position = intersection;
	info.distance = best_t;
	return true;
}
