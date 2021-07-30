#include "LightProbe.h"
#include "ResourceManager.h"

const int LightProbe::s_capture_width = 512;
const int LightProbe::s_capture_height = 512;

LightProbe::LightProbe(const vec3& position, const AABBModule& aabb_)
{
	local_transform.SetPosition(position);
	aabb_module = make_shared<AABBModule>(aabb_);
	aabb_module->SetParent(this);

	cubemap = ResourceManager::GetSingleton().CreateCubemap(s_capture_width, s_capture_height, TextureType::CUBEMAP, true);
	glObjectLabel(GL_TEXTURE, cubemap->id, -1, "lightprobe_cubemap");
}

void LightProbe::Initialize()
{
	//cubemap = ResourceManager::GetSingleton().CreateCubemap(128, 128, TextureType::CUBEMAP, true);
}
