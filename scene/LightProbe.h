#pragma once

#include "AABBModule.h"
#include "CubeMap.h"



class LightProbe : public TransformableModule
{
public:
	CubeMap* cubemap;



	//virtual void Capture() = 0;
	static const int s_capture_width;
	static const int s_capture_height;

	shared_ptr<AABBModule> aabb_module;

	LightProbe(const vec3& postion = vec3{ 0 }, const AABBModule& aabb_ = AABBModule());

	virtual void Initialize();

private:

};

