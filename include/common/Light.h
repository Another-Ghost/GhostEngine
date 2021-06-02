#pragma once
#include "Prerequisite.h"



struct Light
{
	
	vec3 postion;
	vec3 color;
	float intensity;

	Light(vec3 position_ = { 0.f, 0.f, 0.f }, vec3 color_ = {1.f, 1.f, 1.f}, float intensity_ = 100):
		postion(position_), color(color_), intensity(intensity_){}

	virtual ~Light();
};

