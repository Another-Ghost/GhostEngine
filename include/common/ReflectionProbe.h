#pragma once
#include "LightProbe.h"

class ReflectionProbe : LightProbe
{
	ReflectionProbe(const vec3& postion = vec3{ 0 }, const AABBModule& aabb_ = AABBModule());

	void Initialize() override;

	void Capture() override;
};

