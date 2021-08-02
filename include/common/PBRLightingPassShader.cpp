#include "PBRLightingPassShader.h"
#include "RenderManager.h"
#include "AttachmentTexture.h"
#include "GBuffer.h"
#include "SkyboxShader.h"
#include "CubeMap.h"

PBRLightingPassShader::PBRLightingPassShader(const string& vertex_path /*= File::GetShaderPath("pbr_vs")*/, const string& fragment_path /*= File::GetShaderPath("pbr_lighting_pass_f")*/, const string& geometry_path /*= ""*/):
	MVPShader(vertex_path, fragment_path, geometry_path)
{
	Initialize(false);
}

void PBRLightingPassShader::Initialize(bool b_reload)
{
	UseTextureUnit(TextureUnit::g_world_position);
	UseTextureUnit(TextureUnit::g_world_normal);
	UseTextureUnit(TextureUnit::g_basecolor);
	UseTextureUnit(TextureUnit::g_ao_roughness_metalness);
	UseTextureUnit(TextureUnit::g_emissive);
	UseTextureUnit(TextureUnit::irradiance_map);
	UseTextureUnit(TextureUnit::light_prefilter_map);
	UseTextureUnit(TextureUnit::brdf_lut);

	for (const auto& shadow_map_tu : TextureUnit::point_depth_maps)
	{
		UseTextureUnit(shadow_map_tu);
	}

	//UseTextureUnit(TextureUnit::point_depth_map1);
	//UseTextureUnit(TextureUnit::point_depth_map2);

	if (b_reload)
	{
		MVPShader::Initialize(false);
	}
}

void PBRLightingPassShader::Draw()
{
	//glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().lighting_pass_fbo);
	//glClear(GL_COLOR_BUFFER_BIT);


	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTextureBarrier.xhtml
	glTextureBarrier();// To safely read the result of a written texel via a texel fetch in a subsequent drawing command

	shared_ptr<GBuffer> g_buffer = RenderManager::GetSingleton().cur_g_buffer;
	//glBindFramebuffer()

	//g_buffer->Bind();
	TextureUnit::Bind2DTexture(TextureUnit::g_world_position, g_buffer->world_position_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_world_normal, g_buffer->world_normal_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_basecolor, g_buffer->basecolor_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_ao_roughness_metalness, g_buffer->ao_roughness_metalness_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_emissive, g_buffer->emissive_tex);

	glDepthMask(GL_FALSE);	//禁止lighting pass 写入（破坏）深度缓冲（不禁止读取），以免造成skybox无法通过深度测试正确渲染
	//glDisable(GL_DEPTH_TEST);	//禁止写入和读取
	RenderManager::GetSingleton().DrawCaptureQuadMesh(this);
	//glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);


	//g_buffer->Unbind();
	//glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
}


