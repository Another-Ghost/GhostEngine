#include "RenderManager.h"
#include "RenderModule.h"
#include "SceneManager.h"
#include "PointLight.h"
#include "PBRMaterial.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "IBLRenderer.h"

template<> RenderManager* Singleton<RenderManager>::singleton = nullptr;
RenderManager::RenderManager()
{
	capture_view_array =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
}

bool RenderManager::Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	//pbr_shader = new Shader(File::GetShaderPath("basic_vs"), File::GetShaderPath("basic_fs"));
	//GLCall(pbr_shader->Use());
	//pbr_shader->SetInt("albedo_map", 0);
	//pbr_shader->SetInt("normal_map", 1);
	//pbr_shader->SetInt("metalness_map", 2);
	//pbr_shader->SetInt("roughness_map", 3);
	//pbr_shader->SetInt("ao_map", 4);

	ibl_renderer = new IBLRenderer();
	return true;
}

void RenderManager::Update(float dt)
{
	//GLCall(pbr_shader->Use());
	camera = SceneManager::GetSingleton().main_camera;
	//pbr_shader->SetVec3("cam_pos", camera->GetPosition());

	////view_matrix = camera->ViewMatrix();
	////perspective_matrix = camera->PerspectiveMatrix();
	//
	////RenderPBRMaterial(dt);
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ibl_renderer->Update(dt);
}

void RenderManager::RenderPBRMaterial(float dt)
{
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pbr_shader->Use();
	//mat4 view = camera->ViewMatrix();
	pbr_shader->SetMat4("view", camera->ViewMatrix());
	//mat4 projection = camera->PerspectiveMatrix();
	pbr_shader->SetMat4("projection", camera->PerspectiveMatrix());
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

	for (const auto& pair : pbr_mat_module_map)
	{
		PBRMaterial* material = pair.first;

		glActiveTexture(GL_TEXTURE0);
		//cout << "albedo_map ID: " << material->albedo_map->id << "\n";
		GLCall(glBindTexture(GL_TEXTURE_2D, material->albedo_map->texture_id));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material->normal_map->texture_id);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, material->metalness_map->texture_id);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, material->roughness_map->texture_id);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, material->ao_map->texture_id);

		for (const auto& render_module : pair.second)
		{
			Mesh* mesh = render_module->GetMesh();

			//mat4 model = render_module->GetParent()->transform.GetMatrix();
			//pbr_shader->SetMat4("model", model);
			pbr_shader->SetMat4("model", mat4(1));	//?改为从物体获取的model

			glBindVertexArray(mesh->vao_id);
			glDrawElements(GL_TRIANGLE_STRIP, mesh->index_array.size(), GL_UNSIGNED_INT, 0);
		}
	}

}

void RenderManager::ResetRenderArray()
{
	pbr_mat_module_map.clear();
}

void RenderManager::InsertRenderModule(RenderModule* rm)
{
	Material* material = rm->GetMaterial();
	switch (material->type)
	{
	case MaterialType::PBR:
		PBRMaterial* pbr_material = dynamic_cast<PBRMaterial*>(material);
		auto rm_set_ite = pbr_mat_module_map.find(pbr_material);
		if (rm_set_ite == pbr_mat_module_map.end())
		{
			pbr_mat_module_map.emplace(pbr_material, set<RenderModule*>());
		}
		pbr_mat_module_map[pbr_material].emplace(rm);
		break;

	}

}
