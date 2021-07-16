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
#include "common/Node.h"
#include "common/RenderUnit.h"
#include "common/HDRIShader.h"
#include "common/PrefilterShader.h"
#include "common/IrradianceShader.h"
#include "common/BRDFLUTShader.h"
#include "common/SkyboxShader.h" 
#include "common/CubeMapShader.h"
#include "WFTestShader.h"

WFTestRenderer::WFTestRenderer()
{
	Window* window = WindowManager::GetSingleton().s_current_window;

	//1.
	CubeMap* env_cubemap = RenderManager::GetSingleton().GetSkybox();
	//2. create an irradiance cubemap, and re-scale capture FBO to irradiance scale.

	irradiance_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP));
	irradiance_cubemap->width = 32;	//因为每一个点是卷积后的结果，丢失了大部分高频细节，所以可以以较低的分辨率存储，并让 OpenGL 的线性滤波（GL_LINEAR）完成大部分工作
	irradiance_cubemap->height = 32;
	//irradiance_cubemap->min_filter_param = GL_LINEAR;
	irradiance_cubemap->b_genarate_mipmap = false;
	irradiance_cubemap->Buffer();

	IrradianceShader* irradiance_shader = new IrradianceShader();
	irradiance_shader->RenderEnvIrradianceCubeMap(irradiance_cubemap, env_cubemap->id);


	//3.create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	prefilter_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP)); // be sure to set minification filter to mip_linear 
	prefilter_cubemap->width = 128;
	prefilter_cubemap->height = 128;
	prefilter_cubemap->b_genarate_mipmap = true;
	prefilter_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	prefilter_cubemap->Buffer(); 	// be sure to generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.

	PrefilterShader* prefilter_shader = new PrefilterShader();
	prefilter_shader->RenderPrefilterCubeMap(prefilter_cubemap, env_cubemap->id);



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
	pbr_shader = new WFTestShader(File::GetShaderPath("pbr_vs"), File::GetShaderPath("kulla_conty_pbr_f"));

	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	//pbr_shader->SetVec3("albedo", vec3(1.f, 0.f, 0.f));
	//pbr_shader->SetFloat("ao", 1.f);


	//6.
	skybox_shader = new SkyboxShader();


}

void WFTestRenderer::Update(float dt)
{
	//Camera* camera = RenderManager::GetSingleton().camera;

	//pbr_shader->SetProjectionMatrix(camera->PerspectiveMatrix());	//用观察者模式订阅机制改成只在初始化时设置一次，在camera更新projection matrix时通知订阅者
	//pbr_shader->SetViewMatrix(camera->ViewMatrix());
	//pbr_shader->SetCameraPosition(camera->GetPosition());

	//pbr_shader->SetPointLightArray(SceneManager::GetSingleton().light_array);

	//pbr_shader->SetPointLight();

	for (const auto& pair : RenderManager::GetSingleton().pbr_mat_unit_map)
	{
		PBRMaterial* material = pair.first;

		//pbr_shader->BindBaseColorMap(material->basecolor_map->id);
		//pbr_shader->BindNormalMap(material->normal_map->id);
		//pbr_shader->BindMetalnessMap(material->metalness_map->id);
		//pbr_shader->BindRoughnessMap(material->roughness_map->id);
		//pbr_shader->BindAmbientOcclusionMap(material->ao_map->id);
		pbr_shader->BindMaterial(material);

		TextureUnit::BindCubemapTexture(TextureUnit::irradiance_map, irradiance_cubemap);
		TextureUnit::BindCubemapTexture(TextureUnit::light_prefilter_map, prefilter_cubemap);
		TextureUnit::Bind2DTexture(TextureUnit::brdf_lut, RenderManager::GetSingleton().GetBRDFLUT());
		//pbr_shader->BindEnvDiffuseIrradianceMap(irradiance_cubemap->id);
		//pbr_shader->BindEnvSpecularPrefilterMap(prefilter_cubemap->id);
		//pbr_shader->BindEnvSpecularBRDFLUT(RenderManager::GetSingleton().GetBRDFLUT()->id);
		//pbr_shader->BindSSDirectionalAlbedoLUT(ssda_lut_texture->id);

		int column_num = 1;

		for (const auto& render_unit : pair.second)
		{
			Mesh* mesh = render_unit->GetMesh();
			for (int col = 0; col < column_num; ++col) {
				
				mat4 model = render_unit->GetParent()->GetWorldTransform().GetMatrix();	//? 改为通过RootRenderModule成员函数直接获取transform
				model = translate(model, vec3((col - column_num / 2) * 2.5f, 0.f, 0.f));
				pbr_shader->SetModelMatrix(model);

				pbr_shader->SetRoughness(glm::clamp((float)col / column_num, 0.0f, 1.f));

				mesh->Draw(pbr_shader);
			}
		}
	}

	//skybox_shader->RenderSkybox(RenderManager::GetSingleton().GetSkybox()->id);
}

void WFTestRenderer::OnKeyPressed(Window* window)
{
	GLFWwindow* glfw_window = window->GetGLFWWindow();
	if (glfwGetKey(glfw_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		pbr_shader->Reload();
	}
}
