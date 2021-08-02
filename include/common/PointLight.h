#pragma once
#include "Light.h"
struct PointLight : Light
{
	PointLight();

	CubeMap* shadow_cubemap;
};

