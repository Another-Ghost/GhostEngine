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

PBRDeferRenderer::PBRDeferRenderer()
{
	//1.
	CubeMap* env_cubemap = RenderManager::GetSingleton().GetSkybox();
	//2. create an irradiance cubemap, and re-scale capture FBO to irradiance scale.

	irradiance_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateCubemap(32, 32, TextureType::CUBEMAP));	//因为每一个点是卷积后的结果，丢失了大部分高频细节，所以可以以较低的分辨率存储，并让 OpenGL 的线性滤波（GL_LINEAR）完成大部分工作
	//irradiance_cubemap->min_filter_param = GL_LINEAR;
	irradiance_cubemap->b_genarate_mipmap = false;
	irradiance_cubemap->Buffer();

	IrradianceShader* irradiance_shader = new IrradianceShader();
	irradiance_shader->RenderEnvIrradianceCubeMap(irradiance_cubemap, env_cubemap->id);


	//3.create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	prefilter_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateCubemap(128, 128, TextureType::CUBEMAP)); // be sure to set minification filter to mip_linear 
	prefilter_cubemap->b_genarate_mipmap = true;
	prefilter_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	prefilter_cubemap->Buffer(); 	// be sure to generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.

	PrefilterShader* prefilter_shader = new PrefilterShader();
	prefilter_shader->RenderPrefilterCubeMap(prefilter_cubemap, env_cubemap->id);

	//4.
	LDRTextureFile* brdf_lut_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("system/brdf_lut.png")));
	brdf_lut = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::EMPTY2D, true, brdf_lut_file);

	pbr_geometry_pass_shader = make_unique<PBRShader>(File::GetShaderPath("pbr_geometry_pass_v"), File::GetShaderPath("pbr_geometry_pass_f"));
	pbr_lighting_pass_shader = make_unique<PBRLightingPassShader>();
}

void PBRDeferRenderer::Update(float dt)
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

/*Lighting Pass*/

	TextureUnit::BindCubemapTexture(TextureUnit::irradiance_map, irradiance_cubemap);
	TextureUnit::BindCubemapTexture(TextureUnit::light_prefilter_map, prefilter_cubemap);
	TextureUnit::Bind2DTexture(TextureUnit::brdf_lut, brdf_lut);
	//https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTextureBarrier.xhtml

	//shared_ptr<GBuffer> g_buffer = RenderManager::GetSingleton().g_buffer;

	TextureUnit::Bind2DTexture(TextureUnit::g_world_position, g_buffer->world_position_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_world_normal, g_buffer->world_normal_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_basecolor, g_buffer->basecolor_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_ao_roughness_metalness, g_buffer->ao_roughness_metalness_tex);
	TextureUnit::Bind2DTexture(TextureUnit::g_emissive, g_buffer->emissive_tex);

	glTextureBarrier();// To safely read the result of a written texel via a texel fetch in a subsequent drawing command

	glDepthMask(GL_FALSE);	//禁止lighting pass 写入（破坏）深度缓冲（不禁止读取），以免造成skybox无法通过深度测试正确渲染
	//glDisable(GL_DEPTH_TEST);	//禁止写入和读取
	RenderManager::GetSingleton().DrawCaptureQuadMesh(&*pbr_lighting_pass_shader);
	//glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	pbr_lighting_pass_shader->Draw();


/*Skybox*/

	//glBindFramebuffer(GL_FRAMEBUFFER, lighting_pass_fbo);
	RenderManager::GetSingleton().skybox_shader->RenderSkybox(RenderManager::GetSingleton().GetSkybox()->id);
	//glBindFramebuffer(GL_FRAMEBUFFER, RenderManager::GetSingleton().GetCurrentOutputFrameBuffer());

	g_buffer->Unbind();


}
