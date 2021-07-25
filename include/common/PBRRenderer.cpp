#include "PBRRenderer.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "WindowManager.h"
#include "RenderUnit.h"
#include "EquirectangularMap.h"
#include "PBRLightingPassShader.h"
#include "HDRIShader.h"
#include "PrefilterShader.h"
#include "IrradianceShader.h"
#include "BRDFLUTShader.h"



PBRRenderer::PBRRenderer()
{
	Window* window = WindowManager::GetSingleton().s_current_window;

	//1.
	CubeMap* env_cubemap = RenderManager::GetSingleton().GetSkybox();
	//2. create an irradiance cubemap, and re-scale capture FBO to irradiance scale.

	irradiance_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateCubeMap(32, 32, TextureType::CUBEMAP));	//因为每一个点是卷积后的结果，丢失了大部分高频细节，所以可以以较低的分辨率存储，并让 OpenGL 的线性滤波（GL_LINEAR）完成大部分工作
	//irradiance_cubemap->min_filter_param = GL_LINEAR;
	irradiance_cubemap->b_genarate_mipmap = false;
	irradiance_cubemap->Buffer();

	IrradianceShader* irradiance_shader = new IrradianceShader();
	irradiance_shader->RenderEnvIrradianceCubeMap(irradiance_cubemap, env_cubemap->id);


	//3.create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	prefilter_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateCubeMap(128, 128, TextureType::CUBEMAP)); // be sure to set minification filter to mip_linear 
	prefilter_cubemap->b_genarate_mipmap = true;
	prefilter_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	prefilter_cubemap->Buffer(); 	// be sure to generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.

	PrefilterShader* prefilter_shader = new PrefilterShader();
	prefilter_shader->RenderPrefilterCubeMap(prefilter_cubemap, env_cubemap->id);


	//5.
	pbr_shader = new PBRLightingPassShader(File::GetShaderPath("pbr_vs"), File::GetShaderPath("pbr_lighting_pass_f"));

	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	//pbr_shader->SetVec3("albedo", vec3(1.f, 0.f, 0.f));
	//pbr_shader->SetFloat("ao", 1.f);





}

void PBRRenderer::Update(float dt)
{

	for (const auto& pair : RenderManager::GetSingleton().pbr_mat_unit_map)
	{
		PBRMaterial* material = pair.first;

		pbr_shader->BindMaterial(material);

		TextureUnit::BindCubemapTexture(TextureUnit::irradiance_map, irradiance_cubemap);
		TextureUnit::BindCubemapTexture(TextureUnit::light_prefilter_map, prefilter_cubemap);
		TextureUnit::Bind2DTexture(TextureUnit::brdf_lut, RenderManager::GetSingleton().GetBRDFLUT());

		//std::sort()
		for (const auto& render_unit : pair.second)
		{
			Mesh* mesh = render_unit->GetMesh();
			mat4 model = render_unit->GetParent()->GetWorldTransform().GetMatrix();
			pbr_shader->SetModelMatrix(model);

			mesh->Draw(pbr_shader);

		}
	}

	
}

void PBRRenderer::OnKeyPressed(Window* window)
{
	GLFWwindow* glfw_window = window->GetGLFWWindow();
	if (glfwGetKey(glfw_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		pbr_shader->Reload();
	}
}

bool PBRRenderer::RenderUnitCompare(RenderUnit* ru_a, RenderUnit* ru_b)
{

	return false;
}
