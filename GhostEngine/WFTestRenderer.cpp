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
	env_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP));	//? ��Ϊ��SceneManager�л�ȡ
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
	irradiance_cubemap->width = 32;	//��Ϊÿһ�����Ǿ����Ľ������ʧ�˴󲿷ָ�Ƶϸ�ڣ����Կ����Խϵ͵ķֱ��ʴ洢������ OpenGL �������˲���GL_LINEAR����ɴ󲿷ֹ���
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
	//brdf_lut_texture->min_filter_param = GL_LINEAR;	//? ��һ�����ó�GL_LINEAR_MIPMAP_LINEAR���ڵĸ���ԭ�򣬷·���û���������
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

	pbr_shader->SetProjectionMatrix(camera->PerspectiveMatrix());	//�ù۲���ģʽ���Ļ��Ƹĳ�ֻ�ڳ�ʼ��ʱ����һ�Σ���camera����projection matrixʱ֪ͨ������
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
				mat4 model = root_rm->GetParentUnit()->transform.GetMatrix();	//? ��Ϊͨ��RootRenderModule��Ա����ֱ�ӻ�ȡtransform
				model = translate(model, vec3((col - column_num / 2) * 2.5f, 0.f, 0.f));
				pbr_shader->SetModelMatrix(model);

				pbr_shader->SetRoughness(glm::clamp((float)col / column_num, 0.0f, 1.f));

				mesh->Draw();
			}
		}
	}

	skybox_shader->RenderSkybox(env_cubemap->texture_id);
}
