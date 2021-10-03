#include "Light.h"
#include "RenderManager.h"

void Light::SetPosition(const vec3& pos)
{
	postion = pos; 
	if (b_enabled)
	{
		RenderManager::GetSingleton().UpdateLightArray();
	}
}

void Light::SetColor(const vec3& color_)
{
	color = color_;
	if (b_enabled)
	{
		RenderManager::GetSingleton().UpdateLightArray();
	}
}

Light::~Light()
{
}
