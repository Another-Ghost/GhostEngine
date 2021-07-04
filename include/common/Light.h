#pragma once
#include "Prerequisite.h"



struct Light
{
	vec3 postion;
	vec3 color;
	float intensity;

	Light(vec3 position_ = { 0.f, 0.f, 0.f }, vec3 color_ = {1.f, 1.f, 1.f}, float intensity_ = 100):
		postion(position_), color(color_), intensity(intensity_), b_enabled(true){}

	bool b_enabled;

	void SetPosition(const vec3& pos);
	vec3 GetPosition() { return postion; }
	void SetColor(const vec3& color_);
	vec3 GetColor(){ return color; }

	virtual ~Light();
};

