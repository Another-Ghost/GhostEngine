#include "RenderManager.h"
#include "SceneManager.h"
#include "PointLight.h"
#include "PBRMaterial.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

template<> RenderManager* Singleton<RenderManager>::singleton = nullptr;
RenderManager::RenderManager()
{

}

bool RenderManager::Initialize()
{
	pbr_shader = new Shader(File::GetShaderPath("basic_vs"), File::GetShaderPath("basic_fs"));
	return true;
}

void RenderManager::Update(float dt)
{
	camera = SceneManager::GetSingleton().main_camera;
	pbr_shader->SetVec3("cam_pos", camera->transform.GetPosition());
	//view_matrix = camera->ViewMatrix();
	//perspective_matrix = camera->PerspectiveMatrix();
	RenderPBRMaterial(dt);
}

void RenderManager::RenderPBRMaterial(float dt)
{
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const int N = SceneManager::GetSingleton().light_array.size();
	for (unsigned int i = 0; i < N; ++i)
	{
		vec3 pos = SceneManager::GetSingleton().light_array[i]->postion;
		vec3 color = SceneManager::GetSingleton().light_array[i]->color;
		float intensity = SceneManager::GetSingleton().light_array[i]->intensity;	//? 考虑也传进去

		pbr_shader->SetVec3("light_pos_array[" + std::to_string(i) + "]", pos);
		pbr_shader->SetVec3("light_color_array[" + std::to_string(i) + "]", color*intensity);
	}

	pbr_shader->Use();
	pbr_shader->SetMat4("view", camera->ViewMatrix());
	pbr_shader->SetMat4("projection", camera->ViewMatrix());


	for (const auto& pair : pbr_mat_module_map)
	{
		PBRMaterial* material = pair.first;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material->specular_map->id);
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
			Mesh* mesh = render_module->mesh;

			mat4 model = render_module->GetParent()->transform.GetMatrix();
			pbr_shader->SetMat4("model", model);

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
	Material* material = rm->material;
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
