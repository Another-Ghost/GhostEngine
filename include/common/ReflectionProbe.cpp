#include "ReflectionProbe.h"
#include "RenderManager.h"
#include "PrefilterShader.h"
#include "IrradianceShader.h"
#include "ResourceManager.h"

ReflectionProbe::ReflectionProbe(const vec3& position, const AABBModule& aabb_):
	LightProbe(position, aabb_)
{
	prefilter_cubemap = ResourceManager::GetSingleton().CreateCubemap(128, 128, TextureType::CUBEMAP, false);
	prefilter_cubemap->b_genarate_mipmap = true;
	prefilter_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	prefilter_cubemap->Buffer();
	glObjectLabel(GL_TEXTURE, prefilter_cubemap->id, -1, "probe_prefilter_cubemap");

	irradiance_cubemap = ResourceManager::GetSingleton().CreateCubemap(32, 32, TextureType::CUBEMAP, true);
	glObjectLabel(GL_TEXTURE, prefilter_cubemap->id, -1, "probe_irradiance_cubemap");
}

void ReflectionProbe::Initialize()
{

	RenderManager::GetSingletonPtr()->prefilter_shader->RenderPrefilterCubeMap(prefilter_cubemap, cubemap->id);

	RenderManager::GetSingleton().irradiance_shader->RenderEnvIrradianceCubeMap(irradiance_cubemap, cubemap->id);
}


void ReflectionProbe::RenderPrefilterMap()
{

}
