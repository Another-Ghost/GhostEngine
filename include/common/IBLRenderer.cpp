#include "IBLRenderer.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Window.h"
#include "WindowManager.h"
#include "PBRMaterial.h"
#include "RenderModule.h"
#include "Light.h"
#include "EquirectangularMap.h"
#include "RootRenderModule.h"
#include "Node.h"
#include "PBRShader.h"
#include "HDRIShader.h"
#include "PrefilterShader.h"
#include "IrradianceShader.h"
#include "BRDFLUTShader.h"
#include "SkyboxShader.h"
#include "RenderUnit.h"

IBLRenderer::IBLRenderer()
{
	Window* window = WindowManager::GetSingleton().current_window;

	//1.
	env_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP));	//? 改为从SceneManager中获取
	env_cubemap->width = 512;
	env_cubemap->height = 512;
	env_cubemap->b_genarate_mipmap = false;
	env_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	env_cubemap->Buffer();
	HDRTextureFile* hdr_file = dynamic_cast<HDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("hdr/sky0025.hdr"), TextureFileType::HDR));
	equirectanguler_map = dynamic_cast<EquirectangularMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::EQUIRECTANGULARMAP, hdr_file));
	equirectanguler_map->Buffer();
	
	HDRIShader* hdri_cubemap_shader = new HDRIShader();
	hdri_cubemap_shader->RenderCubeMap(env_cubemap, equirectanguler_map->texture_id);


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
	brdf_lut_texture->data_format = GL_RG;
	brdf_lut_texture->width = 512;
	brdf_lut_texture->height = 512;
	//brdf_lut_texture->min_filter_param = GL_LINEAR;	//? 查一下设置成GL_LINEAR_MIPMAP_LINEAR会变黑的根本原因，仿佛是没了漫反射光
	brdf_lut_texture->b_genarate_mipmap = false;
	brdf_lut_texture->Buffer();

	BRDFLUTShader* brdflut_shader = new BRDFLUTShader();
	brdflut_shader->RenderBRDFLUT(brdf_lut_texture);


	//5.
	pbr_shader = new PBRShader();

	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	//pbr_shader->SetVec3("albedo", vec3(1.f, 0.f, 0.f));
	//pbr_shader->SetFloat("ao", 1.f);


	//6.
	skybox_shader = new SkyboxShader();
}

void IBLRenderer::Update(float dt)
{
	//Camera* camera = RenderManager::GetSingleton().camera;

	//pbr_shader->SetProjectionMatrix(camera->PerspectiveMatrix());	//用观察者模式订阅机制改成只在初始化时设置一次，在camera更新projection matrix时通知订阅者
	//pbr_shader->SetViewMatrix(camera->ViewMatrix());
	//pbr_shader->SetCameraPosition(camera->GetPosition());

	pbr_shader->SetCamera(RenderManager::GetSingleton().camera);

	pbr_shader->SetPointLightArray(SceneManager::GetSingleton().light_array);


	for (const auto& pair : RenderManager::GetSingleton().pbr_mat_unit_map)
	{
		PBRMaterial* material = pair.first;

		pbr_shader->BindBaseColorMap(material->basecolor_map->texture_id);
		pbr_shader->BindNormalMap(material->normal_map->texture_id);
		pbr_shader->BindMetalnessMap(material->metalness_map->texture_id);
		pbr_shader->BindRoughnessMap(material->roughness_map->texture_id);
		pbr_shader->BindAmbientOcclusionMap(material->ao_map->texture_id);
		pbr_shader->BindEnvDiffuseIrradianceMap(irradiance_cubemap->texture_id);
		pbr_shader->BindEnvSpecularPrefilterMap(prefilter_cubemap->texture_id);
		pbr_shader->BindEnvSpecularBRDFLUT(brdf_lut_texture->texture_id);


		for (const auto& render_unit : pair.second)
		{
			Mesh* mesh = render_unit->GetMesh();

			RootRenderModule* root_rm = dynamic_cast<RootRenderModule*>(render_unit);
			mat4 model = root_rm->GetParentNode()->GetWorldTransform().GetMatrix();	//? 改为通过RootRenderModule成员函数直接获取transform
			//mat4 model = render_module->GetParent()->transform.GetMatrix();
			pbr_shader->SetModelMatrix(model);

			mesh->Draw();
		}
	}

	skybox_shader->RenderSkybox(env_cubemap->texture_id);
}

