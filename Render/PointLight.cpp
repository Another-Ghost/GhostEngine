#include "PointLight.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "CubeMap.h"

PointLight::PointLight()
{
	shadow_cubemap = ResourceManager::GetSingleton().CreateCubemap(RenderManager::GetSingleton().point_shadow_width, RenderManager::GetSingleton().point_shadow_height);
	glObjectLabel(GL_TEXTURE, shadow_cubemap->id, -1, "point_shadow_map");
	shadow_cubemap->internal_format = GL_DEPTH_COMPONENT;
	shadow_cubemap->data_format = GL_DEPTH_COMPONENT;
	shadow_cubemap->Buffer();
}
