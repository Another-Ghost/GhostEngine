#include "Light.h"

void Light::SetPosition(const vec3& pos)
{
	postion = pos; 

}

void Light::SetColor(const vec3& color_)
{
	color = color_;
}

Light::~Light()
{
}
