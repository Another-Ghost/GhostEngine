#include "LightProbe.h"


LightProbe::LightProbe(const vec3& position, const AABBModule& aabb_)
{
	local_transform.SetPosition(position);
	aabb = make_shared<AABBModule>(aabb_);
}
