#pragma once
#include "Light.h"
#include "CubeMap.h"

struct PointLight : Light
{
	PointLight();

	CubeMap* shadow_cubemap;
};

