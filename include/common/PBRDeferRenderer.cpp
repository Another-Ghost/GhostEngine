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

PBRDeferRenderer::PBRDeferRenderer()
{
	WindowManager::GetSingleton().s_current_window->AddEventListener(this);
	//1.
	CubeMap* env_cubemap = RenderManager::GetSingleton().GetSkybox();
	//2. create an irradiance cubemap, and re-scale capture FBO to irradiance scale.

	irradiance_cubemap = ResourceManager::GetSingleton().CreateCubemap(32, 32, TextureType::CUBEMAP);	//��Ϊÿһ�����Ǿ�����Ľ������ʧ�˴󲿷ָ�Ƶϸ�ڣ����Կ����Խϵ͵ķֱ��ʴ洢������ OpenGL �������˲���GL_LINEAR����ɴ󲿷ֹ���
	//irradiance_cubemap->min_filter_param = GL_LINEAR;
	irradiance_cubemap->b_genarate_mipmap = false;
	irradiance_cubemap->Buffer();

	IrradianceShader* irradiance_shader = new IrradianceShader();
	irradiance_shader->RenderEnvIrradianceCubeMap(irradiance_cubemap, env_cubemap->id);


	//3.create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	prefilter_cubemap = ResourceManager::GetSingleton().CreateCubemap(128, 128, TextureType::CUBEMAP); // be sure to set minification filter to mip_linear 
	prefilter_cubemap->b_genarate_mipmap = true;
	prefilter_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	prefilter_cubemap->Buffer(); 	// be sure to generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.

	PrefilterShader* prefilter_shader = new PrefilterShader();
	prefilter_shader->RenderPrefilterCubeMap(prefilter_cubemap, env_cubemap->id);

	//4.
	LDRTextureFile* brdf_lut_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("system/brdf_lut.png")));
	brdf_lut = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::EMPTY2D, true, brdf_lut_file);

	pbr_geometry_pass_shader = make_shared<PBRShader>(File::GetShaderPath("pbr_geometry_pass_v"), File::GetShaderPath("pbr_geometry_pass_f"));
	pbr_lighting_pass_shader = make_shared<PBRLightingPassShader>();
	//lighting_prerender_shader = make_shared<PBRLightingPassShader>(File::GetShaderPath("basic_v"), File::GetShaderPath("pbr_lighting_pass_prerender_f"));
	direct_lighting_shader = make_shared<PBRLightingPassShader>(File::GetShaderPath("basic_v"), File::GetShaderPath("pbr_direct_lighting_f"));
	//indirect_lighting_shader = make_shared<PBRLightingPassShader>(File::GetShaderPath("pbr_geometry_pass_v"), File::GetShaderPath("pbr_indirect_lighting_f"));

}

void PBRDeferRenderer::Update(float dt)
{
	//RenderDirectLight();

	Render();

//	shared_ptr<GBuffer> g_buffer = RenderManager::GetSingleton().cur_g_buffer;
//	g_buffer->Bind();
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
///*Geometry Pass*/
//
//	auto comp = [](const std::pair<float, PBRMaterial*>& a, const std::pair<float, PBRMaterial*>& b) {return a.first < b.first; };	//�󶥶ѣ���Ϊ����ȾcameraZֵ��ģ���ǰ�ģ�
//
//	priority_queue<std::pair<float, PBRMaterial*>, vector<std::pair<float, PBRMaterial*>>, decltype(comp)> heap(comp);
//
//	for (const auto& pair : RenderManager::GetSingleton().pbr_mat_unit_map)
//	{
//		heap.emplace((*pair.second.begin())->GetCameraZDistance(), pair.first);
//	}
//
//
//	while (!heap.empty())
//	{
//		//std::pair<float, PBRMaterial*> fp = heap.top();
//		const auto& material = heap.top().second;
//		pbr_geometry_pass_shader->BindMaterial(material);
//
//		const auto& ru_set = RenderManager::GetSingleton().pbr_mat_unit_map[material];
//		for (const auto& render_unit : ru_set)
//		{
//			Mesh* mesh = render_unit->GetMesh();
//			mat4 model = render_unit->GetParent()->GetWorldTransform().GetMatrix();
//			pbr_geometry_pass_shader->SetModelMatrix(model);
//
//			mesh->Draw(&*pbr_geometry_pass_shader);			
//		}
//		heap.pop();
//	}
//
///*Lighting Pass*/
//	if (!RenderManager::GetSingleton().b_prerendered)
//	{
//		TextureUnit::BindCubemapTexture(TextureUnit::irradiance_map, irradiance_cubemap);
//		TextureUnit::BindCubemapTexture(TextureUnit::light_prefilter_map, prefilter_cubemap);
//	}
//	else
//	{
//		TextureUnit::BindCubemapTexture(TextureUnit::irradiance_map, RenderManager::GetSingleton().blended_irradiance_cubemap);
//		TextureUnit::BindCubemapTexture(TextureUnit::light_prefilter_map, RenderManager::GetSingleton().blended_prefilter_cubemap);
//
//	}
//
//	TextureUnit::Bind2DTexture(TextureUnit::brdf_lut, brdf_lut);
//	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTextureBarrier.xhtml
//
//	//shared_ptr<GBuffer> g_buffer = RenderManager::GetSingleton().g_buffer;
//
//	TextureUnit::Bind2DTexture(TextureUnit::g_world_position, g_buffer->world_position_tex);
//	TextureUnit::Bind2DTexture(TextureUnit::g_world_normal, g_buffer->world_normal_tex);
//	TextureUnit::Bind2DTexture(TextureUnit::g_basecolor, g_buffer->basecolor_tex);
//	TextureUnit::Bind2DTexture(TextureUnit::g_ao_roughness_metalness, g_buffer->ao_roughness_metalness_tex);
//	TextureUnit::Bind2DTexture(TextureUnit::g_emissive, g_buffer->emissive_tex);
//
//	
//	const vector<PointLight*> point_lights = SceneManager::GetSingletonPtr()->point_light_array;
//	for (int i = 0; i < point_lights.size(); ++i)
//	{
//		TextureUnit::BindCubemapTexture(TextureUnit::point_depth_maps[i], point_lights[i]->shadow_cubemap);
//	}
//
//	//TextureUnit::BindCubemapTexture(TextureUnit::point_depth_map1, )
//
//	glTextureBarrier();// To safely read the result of a written texel via a texel fetch in a subsequent drawing command
//
//	glDepthMask(GL_FALSE);	//��ֹlighting pass д�루�ƻ�����Ȼ��壨����ֹ��ȡ�����������skybox�޷�ͨ����Ȳ�����ȷ��Ⱦ
//	//glDisable(GL_DEPTH_TEST);	//��ֹд��Ͷ�ȡ
//	
//	if (RenderManager::GetSingleton().b_prerendered)
//	{
//		RenderManager::GetSingleton().DrawCaptureQuadMesh(&*pbr_lighting_pass_shader);
//	}
//	else
//	{
//		//lighting_prerender_shader->SetFloat("shadow_far_plane", RenderManager::GetSingletonPtr()->shadow_renderer->far_plane);
//		RenderManager::GetSingleton().DrawCaptureQuadMesh(&*lighting_prerender_shader);
//	}
//
//	//RenderManager::GetSingleton().DrawCaptureQuadMesh(&*lighting_prerender_shader);
//
//	//glEnable(GL_DEPTH_TEST);
//	glDepthMask(GL_TRUE);
//
//	//pbr_lighting_pass_shader->Draw();
//
//
///*Skybox*/
//
//	//glBindFramebuffer(GL_FRAMEBUFFER, lighting_pass_fbo);
//	RenderManager::GetSingleton().skybox_shader->RenderSkybox(RenderManager::GetSingleton().GetSkybox()->id);
//	//glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());
//
//	g_buffer->Unbind();


}

void PBRDeferRenderer::OnKeyPressed(Window* window)
{
	GLFWwindow* glfw_window = window->GetGLFWWindow();
	if (glfwGetKey(glfw_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		pbr_lighting_pass_shader->Reload();
		direct_lighting_shader->Reload();
	}
}

void PBRDeferRenderer::RnderGeometryPass()
{
	shared_ptr<GBuffer> g_buffer = RenderManager::GetSingleton().cur_g_buffer;
	g_buffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*Geometry Pass*/

	auto comp = [](const std::pair<float, PBRMaterial*>& a, const std::pair<float, PBRMaterial*>& b) {return a.first < b.first; };	//�󶥶ѣ���Ϊ����ȾcameraZֵ��ģ���ǰ�ģ�

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
		g_buffer->Bind();	//? buffer ��ͳһ����

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

void PBRDeferRenderer::RenderDirectLight()
{
	RnderGeometryPass();

	shared_ptr<GBuffer> g_buffer = RenderManager::GetSingleton().cur_g_buffer;
	g_buffer->Bind();

	const vector<PointLight*> point_lights = SceneManager::GetSingletonPtr()->point_light_array;
	for (int i = 0; i < point_lights.size(); ++i)
	{
		TextureUnit::BindCubemapTexture(TextureUnit::point_depth_maps[i], point_lights[i]->shadow_cubemap);

	}
	TextureUnit::Bind2DTexture(TextureUnit::g_world_position, g_buffer->world_position_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_world_normal, g_buffer->world_normal_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_basecolor, g_buffer->basecolor_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_ao_roughness_metalness, g_buffer->ao_roughness_metalness_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_emissive, g_buffer->emissive_tex);


	glDepthMask(GL_FALSE);	//��ֹlighting pass д�루�ƻ�����Ȼ��壨����ֹ��ȡ�����������skybox�޷�ͨ����Ȳ�����ȷ��Ⱦ

	RenderManager::GetSingleton().DrawCaptureQuadMesh(&*direct_lighting_shader);

	glDepthMask(GL_TRUE);

/*Skybox*/
	RenderManager::GetSingleton().skybox_shader->RenderSkybox(RenderManager::GetSingleton().GetSkybox()->id);

	g_buffer->Unbind();

}

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
	TextureUnit::BindCubemapTexture(TextureUnit::irradiance_map, RenderManager::GetSingleton().blended_irradiance_cubemap);
	TextureUnit::BindCubemapTexture(TextureUnit::light_prefilter_map, RenderManager::GetSingleton().blended_prefilter_cubemap);
	TextureUnit::Bind2DTexture(TextureUnit::brdf_lut, brdf_lut);
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTextureBarrier.xhtml
	TextureUnit::Bind2DTexture(TextureUnit::g_world_position, g_buffer->world_position_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_world_normal, g_buffer->world_normal_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_basecolor, g_buffer->basecolor_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_ao_roughness_metalness, g_buffer->ao_roughness_metalness_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_emissive, g_buffer->emissive_tex);

	glDepthMask(GL_FALSE);	//��ֹlighting pass д�루�ƻ�����Ȼ��壨����ֹ��ȡ�����������skybox�޷�ͨ����Ȳ�����ȷ��Ⱦ
	RenderManager::GetSingleton().DrawCaptureQuadMesh(&*pbr_lighting_pass_shader);
	
	//RenderManager::GetSingleton().DrawCaptureQuadMesh(&*direct_lighting_shader);
	glDepthMask(GL_TRUE);

/*Skybox*/
	RenderManager::GetSingleton().skybox_shader->RenderSkybox(RenderManager::GetSingleton().GetSkybox()->id);
	g_buffer->Unbind();
}