#include "IBLRenderer.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "SceneManager.h"
#include "HDRTexture.h"
#include "CubeMap.h"
#include "CubeGeometryMeshFactory.h"
#include "Camera.h"
#include "Window.h"
#include "WindowManager.h"
#include "PBRMaterial.h"
#include "RenderModule.h"
#include "Light.h"


IBLRenderer::IBLRenderer()
{
	background_shader = new Shader(File::GetShaderPath("background_vs"), File::GetShaderPath("background_fs"));
	GLCall(background_shader->Use());
	background_shader->SetInt("environment_map", 0);

	//1.
	glGenFramebuffers(1, &capture_fbo);
	glGenRenderbuffers(1, &capture_rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, capture_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, capture_rbo);	//附加一个深度纹理到帧缓冲对象中

	env_cubemap = ResourceManager::GetSingleton().CreateCubeMap(512, 512);	//? 改为从SceneManager中获取
	HDRTexture* equirectangle_tex = dynamic_cast<HDRTexture*>(ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("hdr/newport_loft.hdr"), TextureType::HDRMAP));
	env_cubemap->equirectangular_texture = equirectangle_tex;


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
	glBindTexture(GL_TEXTURE_2D, env_cubemap->equirectangular_texture->id);

	cube_mesh = dynamic_cast<CubeGeometryMesh*>(ResourceManager::GetSingleton().CreateMesh(CubeGeometryMeshFactory()));

	GLCall(glViewport(0, 0, 512, 512));
	glBindFramebuffer(GL_FRAMEBUFFER, capture_fbo);
	for (int i = 0; i < 6; ++i)
	{
		equirectangular_cubemap_shader->SetMat4("view", capture_view_array[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubemap->texture_id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cube_mesh->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//2.
	irradiance_shader = new Shader(File::GetShaderPath("cubemap_vs"), File::GetShaderPath("irradiance_convolution_fs"));
	irradiance_cubemap = ResourceManager::GetSingleton().CreateCubeMap(32, 32);
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

	//3.
	pbr_shader = new Shader(File::GetShaderPath("basic_vs"), File::GetShaderPath("basic_fs"));
	GLCall(pbr_shader->Use());
	pbr_shader->SetInt("albedo_map", 0);
	pbr_shader->SetInt("normal_map", 1);
	pbr_shader->SetInt("metalness_map", 2);
	pbr_shader->SetInt("roughness_map", 3);
	pbr_shader->SetInt("ao_map", 4);
	//pbr_shader->SetVec3("albedo", vec3(0.5f, 0.f, 0.f));
	//pbr_shader->SetFloat("ao", 1.f);



	Window* window = WindowManager::GetSingleton().current_window;
	glViewport(0, 0, window->GetWidth(), window->GetHeight());

}

void IBLRenderer::Update(float dt)
{
	Camera* camera = RenderManager::GetSingleton().camera;
	pbr_shader->Use();
	pbr_shader->SetMat4("projection", RenderManager::GetSingleton().camera->PerspectiveMatrix());
	pbr_shader->SetMat4("view", camera->ViewMatrix());
	pbr_shader->SetVec3("cam_pos", camera->GetPosition());

	const int N = SceneManager::GetSingleton().light_array.size();
	for (unsigned int i = 0; i < N; ++i)
	{
		vec3 pos = SceneManager::GetSingleton().light_array[i]->postion;
		vec3 color = SceneManager::GetSingleton().light_array[i]->color;
		float intensity = SceneManager::GetSingleton().light_array[i]->intensity;	//? 考虑也传进去

		pbr_shader->SetVec3("light_position_array[" + std::to_string(i) + "]", pos);
		pbr_shader->SetVec3("light_color_array[" + std::to_string(i) + "]", color * intensity);
	}

	for (const auto& pair : RenderManager::GetSingleton().pbr_mat_module_map)
	{
		PBRMaterial* material = pair.first;

		glActiveTexture(GL_TEXTURE0);
		//cout << "albedo_map ID: " << material->albedo_map->id << "\n";
		glBindTexture(GL_TEXTURE_2D, material->albedo_map->id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->normal_map->id);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material->metalness_map->id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, material->roughness_map->id);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, material->ao_map->id);

		for (const auto& render_module : pair.second)
		{
			Mesh* mesh = render_module->GetMesh();

			//mat4 model = render_module->GetParent()->transform.GetMatrix();
			//pbr_shader->SetMat4("model", model);
			pbr_shader->SetMat4("model", mat4(1));	//? 改为从物体获取的model

			mesh->Draw();
		}

	}

	background_shader->Use();
	background_shader->SetMat4("view", camera->ViewMatrix());
	background_shader->SetMat4("projection", camera->PerspectiveMatrix());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubemap->texture_id);
	cube_mesh->Draw();

}
