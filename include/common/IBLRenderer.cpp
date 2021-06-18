#include "IBLRenderer.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "CubeMap.h"
#include "CubeGeometryMeshFactory.h"
#include "Camera.h"
#include "Window.h"
#include "WindowManager.h"
#include "PBRMaterial.h"
#include "RenderModule.h"
#include "Light.h"
#include "EquirectangularMap.h"
#include "QuadGeometryMesh.h"
#include "RootRenderModule.h"
#include "Unit.h"
#include "PBRShader.h"

IBLRenderer::IBLRenderer()
{
	Window* window = WindowManager::GetSingleton().current_window;
	cube_mesh = dynamic_cast<CubeGeometryMesh*>(ResourceManager::GetSingleton().CreateMesh(CubeGeometryMeshFactory()));

	//1.
	glGenFramebuffers(1, &capture_fbo);
	glGenRenderbuffers(1, &capture_rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);	//创建一个深度渲染缓冲对象
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, capture_rbo);	//附加一个深度渲染缓冲对象到当前绑定的帧缓冲对象中

	env_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP));	//? 改为从SceneManager中获取
	env_cubemap->width = 512;
	env_cubemap->height = 512;
	env_cubemap->b_genarate_mipmap = false;
	env_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	env_cubemap->Buffer();
	HDRTextureFile* hdr_file = dynamic_cast<HDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("hdr/sky0025.hdr"), TextureFileType::HDR));
	equirectanguler_map = dynamic_cast<EquirectangularMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::EQUIRECTANGULARMAP, hdr_file));
	equirectanguler_map->Buffer();
	//env_cubemap->equirectangular_texture = equirectangle_tex;

	mat4 capture_projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f);
	vector<mat4> capture_view_array =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	equirectangular_cubemap_shader = new Shader(File::GetShaderPath("cubemap_vs"), File::GetShaderPath("equirectangular_to_cubemap_fs"));
	GLCall(equirectangular_cubemap_shader->Use());
	equirectangular_cubemap_shader->SetInt("equirectangular_map", 0);
	equirectangular_cubemap_shader->SetMat4("projection", capture_projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, equirectanguler_map->texture_id);

	GLCall(glViewport(0, 0, 512, 512));

	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	for (int i = 0; i < 6; ++i)
	{
		equirectangular_cubemap_shader->SetMat4("view", capture_view_array[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap->texture_id, 0);	//颜色渲染到纹理附件，深度渲染至之前创建的深度渲染缓冲对象(RBO)中
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cube_mesh->Draw(); //? 是否能像下文中的一样一次渲染整个cubemap
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, window->GetWidth(), window->GetHeight());
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap->texture_id);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//2. create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
	irradiance_shader = new Shader(File::GetShaderPath("cubemap_vs"), File::GetShaderPath("irradiance_convolution_fs"));
	irradiance_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP));
	irradiance_cubemap->width = 32;	//因为每一个点是卷积后的结果，丢失了大部分高频细节，所以可以以较低的分辨率存储，并让 OpenGL 的线性滤波（GL_LINEAR）完成大部分工作
	irradiance_cubemap->height = 32;
	//irradiance_cubemap->min_filter_param = GL_LINEAR;
	irradiance_cubemap->b_genarate_mipmap = false;
	irradiance_cubemap->Buffer();
	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	
	irradiance_shader->Use();
	irradiance_shader->SetInt("environment_map", 0);
	irradiance_shader->SetMat4("projection", capture_projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap->texture_id);
	
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradiance_shader->SetMat4("view", capture_view_array[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_cubemap->texture_id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cube_mesh->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window->GetWidth(), window->GetHeight());



	//3.create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	prefilter_shader = new Shader(File::GetShaderPath("cubemap_vs"), File::GetShaderPath("prefilter_fs"));
	prefilter_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP)); // be sure to set minification filter to mip_linear 
	prefilter_cubemap->width = 128; 
	prefilter_cubemap->height = 128;
	prefilter_cubemap->b_genarate_mipmap = true;
	prefilter_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
	prefilter_cubemap->Buffer(); 	// be sure to generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.

	prefilter_shader->Use();
	prefilter_shader->SetInt("environment_map", 0);
	prefilter_shader->SetMat4("projection", capture_projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap->texture_id);

	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	unsigned int max_mip_levels = 5;
	for (unsigned int mip = 0; mip < max_mip_levels; ++mip)
	{
		// resize framebuffer according to mip-level size.
		unsigned int mip_width = 128 * std::pow(0.5f, mip);	//大小很关键
		unsigned int mip_height = 128 * std::pow(0.5f, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mip_width, mip_height);
		glViewport(0, 0, mip_width, mip_height);

		float roughness = (float)mip / (float)(max_mip_levels - 1);
		prefilter_shader->SetFloat("roughness", roughness);
		for (int i = 0; i < 6; ++i)
		{
			prefilter_shader->SetMat4("view", capture_view_array[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_cubemap->texture_id, mip); //最后一个参数mip决定渲染到绑定的纹理的哪一层mipmap上

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //? 是否应该被加到每个drawcall之前
			cube_mesh->Draw();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window->GetWidth(), window->GetHeight());


	//4. generate a 2D LUT from the BRDF equations used.
	brdf_shader = new Shader(File::GetShaderPath("brdf_vs"), File::GetShaderPath("brdf_fs"));
	
	brdf_lut_texture = ResourceManager::GetSingleton().CreateTexture(TextureType::EMPTY2D); 	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	brdf_lut_texture->data_format = GL_RG;
	brdf_lut_texture->width = 512;
	brdf_lut_texture->height = 512;
	//brdf_lut_texture->min_filter_param = GL_LINEAR;	//? 查一下设置成GL_LINEAR_MIPMAP_LINEAR会变黑的根本原因，仿佛是没了漫反射光
	brdf_lut_texture->b_genarate_mipmap = false;
	brdf_lut_texture->Buffer();

	// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdf_lut_texture->texture_id, 0);	

	QuadGeometryMesh* quad_mesh = new QuadGeometryMesh();

	glViewport(0, 0, 512, 512);
	brdf_shader->Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quad_mesh->Draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window->GetWidth(), window->GetHeight());


	//5.
	pbr_shader = new PBRShader();

	//pbr_shader = new Shader(File::GetShaderPath("pbr_vs"), File::GetShaderPath("pbr_fs"));
	//GLCall(pbr_shader->Use());
	//pbr_shader->SetInt("albedo_map", 0);
	//pbr_shader->SetInt("normal_map", 1);
	//pbr_shader->SetInt("metalness_map", 2);
	//pbr_shader->SetInt("roughness_map", 3);
	//pbr_shader->SetInt("ao_map", 4);
	//pbr_shader->SetInt("irradiance_map", 5);
	//pbr_shader->SetInt("prefilter_map", 6);
	//pbr_shader->SetInt("brdf_LUT", 7);

	glViewport(0, 0, window->GetWidth(), window->GetHeight());
	//pbr_shader->SetVec3("albedo", vec3(1.f, 0.f, 0.f));
	//pbr_shader->SetFloat("ao", 1.f);


	//6.
	background_shader = new Shader(File::GetShaderPath("background_vs"), File::GetShaderPath("background_fs"));
	GLCall(background_shader->Use());
	background_shader->SetInt("environment_map", 0);
}

void IBLRenderer::Update(float dt)
{
	Camera* camera = RenderManager::GetSingleton().camera;

	pbr_shader->SetProjectionMatrix(camera->PerspectiveMatrix());	//用观察者模式订阅机制改成只在初始化时设置一次，在camera更新projection matrix时通知订阅者
	pbr_shader->SetViewMatrix(camera->ViewMatrix());
	pbr_shader->SetCameraPosition(camera->GetPosition());

	//pbr_shader->Use();
	//pbr_shader->SetMat4("projection", RenderManager::GetSingleton().camera->PerspectiveMatrix());
	//pbr_shader->SetMat4("view", camera->ViewMatrix());
	//pbr_shader->SetVec3("cam_pos", camera->GetPosition());

	pbr_shader->SetPointLightArray(SceneManager::GetSingleton().light_array);

	//const int N = SceneManager::GetSingleton().light_array.size();

	//for (unsigned int i = 0; i < N; ++i)
	//{
	//	vec3 pos = SceneManager::GetSingleton().light_array[i]->postion;
	//	vec3 color = SceneManager::GetSingleton().light_array[i]->color;
	//	float intensity = SceneManager::GetSingleton().light_array[i]->intensity;	//? 考虑也传进去

	//	pbr_shader->SetVec3("light_position_array[" + std::to_string(i) + "]", pos);
	//	pbr_shader->SetVec3("light_color_array[" + std::to_string(i) + "]", color * intensity);
	//}

	for (const auto& pair : RenderManager::GetSingleton().pbr_mat_module_map)
	{
		PBRMaterial* material = pair.first;

		//glActiveTexture(GL_TEXTURE0);
		////cout << "albedo_map ID: " << material->albedo_map->id << "\n";
		//glBindTexture(GL_TEXTURE_2D, material->albedo_map->texture_id);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, material->normal_map->texture_id);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, material->metalness_map->texture_id);
		//glActiveTexture(GL_TEXTURE3);
		//glBindTexture(GL_TEXTURE_2D, material->roughness_map->texture_id);
		//glActiveTexture(GL_TEXTURE4);
		//glBindTexture(GL_TEXTURE_2D, material->ao_map->texture_id);

		//// bind pre-computed IBL data
		//glActiveTexture(GL_TEXTURE5);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_cubemap->texture_id);
		//glActiveTexture(GL_TEXTURE6);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_cubemap->texture_id);
		//glActiveTexture(GL_TEXTURE7);
		//glBindTexture(GL_TEXTURE_2D, brdf_lut_texture->texture_id);

		pbr_shader->BindAlbedoMap(material->albedo_map->texture_id);
		pbr_shader->BindNormalMap(material->normal_map->texture_id);
		pbr_shader->BindMetalnessMap(material->metalness_map->texture_id);
		pbr_shader->BindRoughnessMap(material->roughness_map->texture_id);
		pbr_shader->BindAmbientOcclusionMap(material->ao_map->texture_id);
		pbr_shader->BindEnvDiffuseIrradianceMap(irradiance_cubemap->texture_id);
		pbr_shader->BindEnvSpecularPrefilterMap(prefilter_cubemap->texture_id);
		pbr_shader->BindEnvSpecularBRDFLUT(brdf_lut_texture->texture_id);

		for (const auto& render_module : pair.second)
		{
			Mesh* mesh = render_module->GetMesh();

			RootRenderModule* root_rm = dynamic_cast<RootRenderModule*>(render_module);
			mat4 model = root_rm->GetParentUnit()->transform.GetMatrix();	//? 改为通过RootRenderModule成员函数直接获取transform
			//mat4 model = render_module->GetParent()->transform.GetMatrix();
			//pbr_shader->SetMat4("model", model);
			pbr_shader->SetModelMatrix(mat4(1));

			//pbr_shader->SetMat4("model", mat4(1));	//? 改为从物体获取的model

			mesh->Draw();
		}
	}

	background_shader->Use();
	background_shader->SetMat4("view", camera->ViewMatrix());
	background_shader->SetMat4("projection", camera->PerspectiveMatrix());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap->texture_id);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_cubemap->texture_id);
	cube_mesh->Draw();	//render skybox

}

