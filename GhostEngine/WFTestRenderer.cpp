#include "WFTestRenderer.h"
#include "common/ResourceManager.h"
#include "common/RenderManager.h"
#include "common/SceneManager.h"
#include "common/Camera.h"
#include "common/Window.h"
#include "common/WindowManager.h"
#include "common/PBRMaterial.h"
#include "common/RenderModule.h"
#include "common/Light.h"
#include "common/EquirectangularMap.h"
#include "common/RootRenderModule.h"
#include "common/Unit.h"

#include "common/HDRIShader.h"
#include "common/PrefilterShader.h"
#include "common/IrradianceShader.h"
#include "common/BRDFLUTShader.h"
#include "common/SkyboxShader.h" 
#include "common/CubeMapShader.h"
#include "WFTestShader.h"

WFTestRenderer::WFTestRenderer()
{
	Window* window = WindowManager::GetSingleton().current_window;

	//1.
	env_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP));	//? 改为从SceneManager中获取
	env_cubemap->width = 512;
	env_cubemap->height = 512;
	env_cubemap->b_genarate_mipmap = false;
	env_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	env_cubemap->Buffer();
	//HDRTextureFile* hdr_file = dynamic_cast<HDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("hdr/sky0025.hdr"), TextureFileType::HDR));
	//equirectanguler_map = dynamic_cast<EquirectangularMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::EQUIRECTANGULARMAP, hdr_file));
	//equirectanguler_map->Buffer();

	CubeMapShader* cubemap_shader = new CubeMapShader();
	cubemap_shader->SetColor({ 0.5f, 0.5f, 0.5f });
	cubemap_shader->RenderCubeMap(env_cubemap);


	//2. create an irradiance cubemap, and re-scale capture FBO to irradiance scale.

	irradiance_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP));
	irradiance_cubemap->width = 32;	//因为每一个点是卷积后的结果，丢失了大部分高频细节，所以可以以较低的分辨率存储，并让 OpenGL 的线性滤波（GL_LINEAR）完成大部分工作
	irradiance_cubemap->height = 32;
	//irradiance_cubemap->min_filter_param = GL_LINEAR;
	irradiance_cubemap->b_genarate_mipmap = false;
	irradiance_cubemap->Buffer();

	IrradianceShader* irradiance_shader = new IrradianceShader();
	irradiance_shader->RenderEnvIrradianceCubeMap(irradiance_cubemap, env_cubemap->texture_id);


	//3.create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	prefilter_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP)); // be sure to set minification filter to mip_linear 
	prefilter_cubemap->width = 128;
	prefilter_cubemap->height = 128;
	prefilter_cubemap->b_genarate_mipmap = true;
	prefilter_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	prefilter_cubemap->Buffer(); 	// be sure to generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.

	PrefilterShader* prefilter_shader = new PrefilterShader();
	prefilter_shader->RenderPrefilterCubeMap(prefilter_cubemap, env_cubemap->texture_id);


	//4. generate a 2D LUT from the BRDF equations used.
	brdf_lut_texture = ResourceManager::GetSingleton().CreateTexture(TextureType::EMPTY2D); 	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	brdf_lut_texture->data_format = GL_RGB;
	brdf_lut_texture->width = 512;
	brdf_lut_texture->height = 512;
	//brdf_lut_texture->min_filter_param = GL_LINEAR;	//? 查一下设置成GL_LINEAR_MIPMAP_LINEAR会变黑的根本原因，仿佛是没了漫反射光
	brdf_lut_texture->b_genarate_mipmap = false;
	brdf_lut_texture->Buffer();

	BRDFLUTShader* brdflut_shader = new BRDFLUTShader();
	brdflut_shader->RenderBRDFLUT(brdf_lut_texture);

	//single scattering directional albedo
	ssda_lut_texture = ResourceManager::GetSingleton().CreateTexture(TextureType::EMPTY2D); 	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	ssda_lut_texture->data_format = GL_RGB;
	ssda_lut_texture->width = 512;
	ssda_lut_texture->height = 512;
	ssda_lut_texture->b_genarate_mipmap = false;
	ssda_lut_texture->Buffer();
	BRDFLUTShader* ssda_shader = new BRDFLUTShader(File::GetShaderPath("brdf_vs"), File::GetShaderPath("single_scattering_energy_f"));
	ssda_shader->RenderBRDFLUT(ssda_lut_texture);

	//5.
	pbr_shader = new WFTestShader();

	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	//pbr_shader->SetVec3("albedo", vec3(1.f, 0.f, 0.f));
	//pbr_shader->SetFloat("ao", 1.f);


	//6.
	skybox_shader = new SkyboxShader();
}

void WFTestRenderer::Update(float dt)
{
	Camera* camera = RenderManager::GetSingleton().camera;

	pbr_shader->SetProjectionMatrix(camera->PerspectiveMatrix());	//用观察者模式订阅机制改成只在初始化时设置一次，在camera更新projection matrix时通知订阅者
	pbr_shader->SetViewMatrix(camera->ViewMatrix());
	pbr_shader->SetCameraPosition(camera->GetPosition());


	pbr_shader->SetPointLightArray(SceneManager::GetSingleton().light_array);


	for (const auto& pair : RenderManager::GetSingleton().pbr_mat_module_map)
	{
		PBRMaterial* material = pair.first;

		pbr_shader->BindAlbedoMap(material->albedo_map->texture_id);
		pbr_shader->BindNormalMap(material->normal_map->texture_id);
		pbr_shader->BindMetalnessMap(material->metalness_map->texture_id);
		pbr_shader->BindRoughnessMap(material->roughness_map->texture_id);
		pbr_shader->BindAmbientOcclusionMap(material->ao_map->texture_id);
		pbr_shader->BindEnvDiffuseIrradianceMap(irradiance_cubemap->texture_id);
		pbr_shader->BindEnvSpecularPrefilterMap(prefilter_cubemap->texture_id);
		pbr_shader->BindEnvSpecularBRDFLUT(brdf_lut_texture->texture_id);
		pbr_shader->BindSSDirectionalAlbedoLUT(ssda_lut_texture->texture_id);

		int column_num = 9;

		for (const auto& render_module : pair.second)
		{
			Mesh* mesh = render_module->GetMesh();
			for (int col = 0; col < column_num; ++col) {
				RootRenderModule* root_rm = dynamic_cast<RootRenderModule*>(render_module);
				mat4 model = root_rm->GetParentUnit()->transform.GetMatrix();	//? 改为通过RootRenderModule成员函数直接获取transform
				model = translate(model, vec3((col - column_num / 2) * 2.5f, 0.f, 0.f));
				pbr_shader->SetModelMatrix(model);

				pbr_shader->SetRoughness(glm::clamp((float)col / column_num, 0.0f, 1.f));

				mesh->Draw();
			}
		}
	}

	skybox_shader->RenderSkybox(env_cubemap->texture_id);
}
