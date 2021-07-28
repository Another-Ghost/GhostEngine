#pragma once
#include "Prerequisite.h"
#include "AABBModule.h"
#include "CubeMap.h"

class LightProbe : TransformableModule
{
public:
	CubeMap* cubemap;

	LightProbe(const vec3& postion = vec3{ 0 }, const AABBModule& aabb_ = AABBModule());

	virtual void Initialize() = 0;

private:
	shared_ptr<AABBModule> aabb;
};

