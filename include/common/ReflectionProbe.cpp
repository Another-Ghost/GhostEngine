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

	depth_cubemap = ResourceManager::GetSingleton().CreateCubemap(s_capture_width, s_capture_width);
	glObjectLabel(GL_TEXTURE, depth_cubemap->id, -1, "point_shadow_map");
	depth_cubemap->internal_format = GL_DEPTH_COMPONENT;
	depth_cubemap->data_format = GL_DEPTH_COMPONENT;
	depth_cubemap->Buffer();

	world_pos_cubemap = ResourceManager::GetSingleton().CreateCubemap(s_capture_width, s_capture_width);	//因为需要采样数据，所以position等使用纹理附件而不是渲染缓冲对象附件
	world_pos_cubemap->data_format = GL_RGBA;
	world_pos_cubemap->min_filter_param = GL_NEAREST;
	world_pos_cubemap->mag_filter_param = GL_NEAREST;
	world_pos_cubemap->Buffer();

}

void ReflectionProbe::Initialize()
{

	RenderManager::GetSingletonPtr()->prefilter_shader->RenderPrefilterCubeMap(prefilter_cubemap, cubemap->id);

	RenderManager::GetSingleton().irradiance_shader->RenderEnvIrradianceCubeMap(irradiance_cubemap, cubemap->id);
}


void ReflectionProbe::RenderPrefilterMap()
{

}
