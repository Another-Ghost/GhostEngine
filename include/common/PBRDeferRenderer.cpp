#include "PBRDeferRenderer.h"
#include "ResourceManager.h"
#include "CubeMap.h"
#include "PrefilterShader.h"
#include "IrradianceShader.h"
#include "BRDFLUTShader.h"
#include "RenderManager.h"
#include "GBuffer.h"
#include "SkyboxShader.h"
#include "AttachmentTexture.h"
#include "WindowManager.h"
#include "SceneManager.h"
#include "PointLight.h"
#include "ShadowRenderer.h"
#include "PBRShader.h"
#include "PBRLightingPassShader.h"
#include "ReflectionProbe.h"
#include "RenderModule.h"


PBRDeferRenderer::PBRDeferRenderer()
{
	WindowManager::GetSingleton().s_current_window->AddEventListener(this);

	//4.
	LDRTextureFile* brdf_lut_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("system/brdf_lut.png")));
	brdf_lut = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::EMPTY2D, true, brdf_lut_file);

	pbr_geometry_pass_shader = make_shared<PBRShader>(File::GetShaderPath("pbr_geometry_pass_v"), File::GetShaderPath("pbr_geometry_pass_f"));
	pbr_lighting_pass_shader = make_shared<PBRLightingPassShader>();
	//lighting_prerender_shader = make_shared<PBRLightingPassShader>(File::GetShaderPath("basic_v"), File::GetShaderPath("pbr_lighting_pass_prerender_f"));
	//direct_lighting_shader = make_shared<PBRLightingPassShader>(File::GetShaderPath("basic_v"), File::GetShaderPath("pbr_direct_lighting_f"));
	//indirect_lighting_shader = make_shared<PBRLightingPassShader>(File::GetShaderPath("pbr_geometry_pass_v"), File::GetShaderPath("pbr_indirect_lighting_f"));

}

void PBRDeferRenderer::Update(float dt)
{
	//RenderDirectLight();
	Render();
}

void PBRDeferRenderer::OnKeyPressed(Window* window)
{
	GLFWwindow* glfw_window = window->GetGLFWWindow();
	if (glfwGetKey(glfw_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		pbr_lighting_pass_shader->Reload();
	}
}

void PBRDeferRenderer::RnderGeometryPass()
{
	shared_ptr<GBuffer> g_buffer = RenderManager::GetSingleton().cur_g_buffer;
	g_buffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*Geometry Pass*/

	auto comp = [](const std::pair<float, PBRMaterial*>& a, const std::pair<float, PBRMaterial*>& b) {return a.first < b.first; };	//大顶堆，因为先渲染cameraZ值大的（靠前的）

	priority_queue<std::pair<float, PBRMaterial*>, vector<std::pair<float, PBRMaterial*>>, decltype(comp)> heap(comp);

	for (const auto& pair : RenderManager::GetSingleton().pbr_mat_unit_map)
	{
		heap.emplace((*pair.second.begin())->GetCameraZDistance(), pair.first);
	}


	while (!heap.empty())
	{
		//std::pair<float, PBRMaterial*> fp = heap.top();
		const auto& material = heap.top().second;
		pbr_geometry_pass_shader->BindMaterial(material);
		//g_buffer->Bind();	//? buffer 类统一管理

		const auto& ru_set = RenderManager::GetSingleton().pbr_mat_unit_map[material];
		for (const auto& render_unit : ru_set)
		{
			Mesh* mesh = render_unit->GetMesh();
			mat4 model = render_unit->GetParent()->GetWorldTransform().GetMatrix();
			pbr_geometry_pass_shader->SetModelMatrix(model);

			mesh->Draw(&*pbr_geometry_pass_shader);
		}
		heap.pop();
	}

	g_buffer->Unbind();
}

//void PBRDeferRenderer::RenderDirectLight()
//{
//	RnderGeometryPass();
//
//	shared_ptr<GBuffer> g_buffer = RenderManager::GetSingleton().cur_g_buffer;
//	g_buffer->Bind();
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, g_buffer->color_tex->id, 0);
//
//	const vector<PointLight*> point_lights = SceneManager::GetSingletonPtr()->point_light_array;
//	for (int i = 0; i < point_lights.size(); ++i)
//	{
//		TextureUnit::BindCubemapTexture(TextureUnit::point_depth_maps[i], point_lights[i]->shadow_cubemap);
//	}
//	TextureUnit::Bind2DTexture(TextureUnit::g_world_position, g_buffer->world_position_tex);
//	TextureUnit::Bind2DTexture(TextureUnit::g_world_normal, g_buffer->world_normal_tex);
//	TextureUnit::Bind2DTexture(TextureUnit::g_basecolor, g_buffer->basecolor_tex);
//	TextureUnit::Bind2DTexture(TextureUnit::g_ao_roughness_metalness, g_buffer->ao_roughness_metalness_tex);
//	TextureUnit::Bind2DTexture(TextureUnit::g_emissive, g_buffer->emissive_tex);
//
//
//	glDepthMask(GL_FALSE);	//禁止lighting pass 写入（破坏）深度缓冲（不禁止读取），以免造成skybox无法通过深度测试正确渲染
//
//	RenderManager::GetSingleton().DrawCaptureQuadMesh(&*direct_lighting_shader);
//
//	glDepthMask(GL_TRUE);
//
/////*Skybox*/
////	RenderManager::GetSingleton().skybox_shader->RenderSkybox(RenderManager::GetSingleton().GetSkybox()->id);
////
////	g_buffer->Unbind();
//
//}

void PBRDeferRenderer::Render()
{
	RnderGeometryPass();

	shared_ptr<GBuffer> g_buffer = RenderManager::GetSingleton().cur_g_buffer;
	g_buffer->Bind();

	/*Lighting Pass*/
	const vector<PointLight*> point_lights = SceneManager::GetSingletonPtr()->point_light_array;
	for (int i = 0; i < point_lights.size(); ++i)
	{
		TextureUnit::BindCubemapTexture(TextureUnit::point_depth_maps[i], point_lights[i]->shadow_cubemap);
	}

	const vector<ReflectionProbe*> probes = RenderManager::GetSingletonPtr()->enabled_probes;

	//for (int i = 0; i < probes.size(); ++i)
	//{
		//TextureUnit::BindCubemapTexture(TextureUnit::irradiance_maps[i], probes[i]->irradiance_cubemap);
		//TextureUnit::BindCubemapTexture(TextureUnit::light_prefilter_maps[i], probes[i]->prefilter_cubemap);
		//TextureUnit::BindCubemapTexture(TextureUnit::probe_depth_maps[i], probes[i]->depth_cubemap);
		//TextureUnit::BindCubemapTexture(TextureUnit::probe_depth_maps[i], probes[i]->world_pos_cubemap);
	//}

	const vector<CubeMap*> irradiance_cubemaps = RenderManager::GetSingletonPtr()->irradiance_cubemaps;
	for (int i = 0; i < irradiance_cubemaps.size(); ++i)
	{
		TextureUnit::BindCubemapTexture(TextureUnit::irradiance_maps[i], irradiance_cubemaps[i]);
	}

	const vector<CubeMap*> light_prefilter_maps = RenderManager::GetSingletonPtr()->prefilter_cubemaps;
	for (int i = 0; i < light_prefilter_maps.size(); ++i)
	{
		TextureUnit::BindCubemapTexture(TextureUnit::light_prefilter_maps[i], light_prefilter_maps[i]);
	}


	TextureUnit::Bind2DTexture(TextureUnit::brdf_lut, brdf_lut);
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTextureBarrier.xhtml
	TextureUnit::Bind2DTexture(TextureUnit::g_world_position, g_buffer->world_position_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_world_normal, g_buffer->world_normal_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_basecolor, g_buffer->basecolor_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_ao_roughness_metalness, g_buffer->ao_roughness_metalness_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_emissive, g_buffer->emissive_tex);

	glDepthMask(GL_FALSE);	//禁止lighting pass 写入（破坏）深度缓冲（不禁止读取），以免造成skybox无法通过深度测试正确渲染
	RenderManager::GetSingleton().DrawCaptureQuadMesh(&*pbr_lighting_pass_shader);
	
	//RenderManager::GetSingleton().DrawCaptureQuadMesh(&*direct_lighting_shader);
	glDepthMask(GL_TRUE);

/*Skybox*/
	RenderManager::GetSingleton().skybox_shader->RenderSkybox(RenderManager::GetSingleton().GetSkybox()->id);
	g_buffer->Unbind();
}
