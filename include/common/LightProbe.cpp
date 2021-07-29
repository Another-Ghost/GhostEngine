#include "LightProbe.h"
#include "ResourceManager.h"

LightProbe::LightProbe(const vec3& position, const AABBModule& aabb_)
{
	local_transform.SetPosition(position);
	aabb = make_shared<AABBModule>(aabb_);
	cubemap = ResourceManager::GetSingleton().CreateCubemap(128, 128, TextureType::CUBEMAP, true);
	glObjectLabel(GL_TEXTURE, cubemap->id, -1, "lightprobe_cubemap");
}

void LightProbe::Initialize()
{
	cubemap = ResourceManager::GetSingleton().CreateCubemap(128, 128, TextureType::CUBEMAP, true);
}
