#include "RenderManager.h"
#include "RenderUnit.h"
#include "SceneManager.h"
#include "PointLight.h"
#include "BasicMaterial.h"
#include "PBRMaterial.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "IBLRenderer.h"
#include "CubeGeometryMeshFactory.h"
#include "ResourceManager.h"
#include "CubeMap.h"
#include "TextureFile.h"
#include "EquirectangularMap.h"
#include "HDRIShader.h"
#include "File.h"

template<> RenderManager* Singleton<RenderManager>::singleton = nullptr;
RenderManager::RenderManager():
	b_skybox_initialized(false)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenFramebuffers(1, &capture_fbo);	
	glGenRenderbuffers(1, &capture_rbo);

	capture_view_array =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
	capture_projection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f);
	capture_cube_mesh = dynamic_cast<CubeGeometryMesh*>(ResourceManager::GetSingleton().CreateMesh(CubeGeometryMeshFactory()));

	capture_quad_mesh = new QuadGeometryMesh();

	LDRTextureFile* brdf_lut_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("system/brdf_lut.png"), TextureFileType::LDR));
	brdf_lut = ResourceManager::GetSingleton().CreateTexture(TextureType::EMPTY2D, brdf_lut_file, true);
	//brdf_lut->data_format = GL_RG;

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);


	//vec4 pos_array[2];
	//pos_array[0] = { 0, 0, 10, 0};
	//pos_array[1] = { 5, 0, 10, 0 };
	//vec4 color_array[2];
	//color_array[0] = { 200, 200, 200, 0 };
	//color_array[1] = { 200, 0, 200, 0 };
	glGenBuffers(1, &light_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_ssbo);
	//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(pos_array), pos_array, GL_DYNAMIC_DRAW);	//? GL_STREAM_COPY
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	//glGenBuffers(1, &light_color_ssbo);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_color_ssbo);
	////glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(color_array), color_array, GL_DYNAMIC_DRAW);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, light_color_ssbo);





	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_pos_ssbo);

	////glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_pos_ssbo);

	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_color_ssbo);

}

bool RenderManager::Initialize(Renderer* renderer_)
{
	if (!b_initialized)
	{
		if (renderer_ == nullptr)
		{
			renderer_ = new IBLRenderer();
		}
		current_renderer = renderer_;

		b_initialized = true;
	}

	return b_initialized;
}



void RenderManager::Update(float dt)
{
	UpdateLightArray();
	//GLCall(pbr_shader->Use());
	camera = SceneManager::GetSingleton().main_camera;
	//pbr_shader->SetVec3("cam_pos", camera->GetPosition());

	////view_matrix = camera->ViewMatrix();
	////perspective_matrix = camera->PerspectiveMatrix();
	//
	////RenderPBRMaterial(dt);
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	current_renderer->Update(dt);


}


void RenderManager::UpdateLightArray()
{
	//int n = SceneManager::GetSingleton().light_array.size();
	////unique_ptr<vec4>  pos_array(new vec4[n]);
	//unique_ptr<LightInfo[]> light_info_array(new LightInfo[n]);

	//vector<vec4> light_pos_array;
	//vector<vec4> light_color_array;

	vector<LightInfo> info_array;
	for (const auto& light : SceneManager::GetSingleton().light_array)
	{
		if (light->b_enabled)
		{
			//vec4 pos = { light->postion, 0.f };
			//vec4 color = { light->color * light->intensity, 0.f };
			//light_pos_array.emplace_back(pos);
			//light_color_array.emplace_back(color);
			LightInfo light_info;
			light_info.position = { light->postion, 0.f };
			light_info.color = { light->color * light->intensity, 0.f };
			info_array.emplace_back(light_info);
		}
	}

	//const vector<Light*>& light_array = SceneManager::GetSingleton().light_array;
	//for (int i = 0; i < n; ++i)
	//{
	//	LightInfo light_info;
	//	light_info.position = { light_array[i]->postion, 0.f };
	//	light_info.color = { 200, 200, 200, 0 }; // { light_array[i]->color* light_array[i]->intensity, 0.f };
	//	//light_info.b_rendering = light_array[i]->b_enabled;
	//	light_info_array[i] = light_info;
	//}

	////vec3 pos_array[1];
	////pos_array[0] = light_pos_array[0];

	////vec3 color_array[1]; 
	////color_array[0] = light_color_array[0];

	////vec4 pos_array[2];
	//vec4 pos_array[1];
	//pos_array[0] = { 0, 0, 10, 0 };
	////pos_array[1] = { 5, 0, 10, 0 };

	//unique_ptr<vec4[]>  color_array(new vec4[n]);
	//color_array[0] = { 200, 200, 200, 0 };
	////color_array[1] = { 200, 0, 200, 0 };

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_ssbo);
	//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * light_pos_array.size(), light_pos_array, GL_DYNAMIC_COPY);	//? GL_STREAM_COPY
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightInfo)* info_array.size(), &info_array[0], GL_STREAM_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, light_color_ssbo);
	////glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4) * light_color_array.size(), &light_color_array, GL_DYNAMIC_COPY);
	//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec4)*n, &color_array[0], GL_STREAM_COPY);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, light_color_ssbo);


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
	for (int i = 0; i < N; ++i)
	{
		vec3 pos = SceneManager::GetSingleton().light_array[i]->postion;
		vec3 color = SceneManager::GetSingleton().light_array[i]->color;
		float intensity = SceneManager::GetSingleton().light_array[i]->intensity;	//? 考虑也传进去

		pbr_shader->SetVec3("light_position_array[" + std::to_string(i) + "]", pos);
		pbr_shader->SetVec3("light_color_array[" + std::to_string(i) + "]", color * intensity);
	}

	for (const auto& pair : pbr_mat_unit_map)
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
	pbr_mat_unit_map.clear();
}

void RenderManager::InsertRenderUnit(RenderUnit* ru)
{
	Material* material = ru->GetMaterial();
	if (material->type == MaterialType::BASIC)
	{
		BasicMaterial* basic_mat = dynamic_cast<BasicMaterial*>(material);
		if (basic_mat_unit_map.count(basic_mat))
		{
			basic_mat_unit_map[basic_mat].emplace(ru);
		}
		else
		{
			basic_mat_unit_map.emplace(basic_mat, set<RenderUnit*>());
		}
	}
	else if(material->type == MaterialType::PBR)
	{
		PBRMaterial* pbr_mat = dynamic_cast<PBRMaterial*>(material);
		auto ru_set_ite = pbr_mat_unit_map.find(pbr_mat);
		if (ru_set_ite == pbr_mat_unit_map.end())
		{
			pbr_mat_unit_map.emplace(pbr_mat, set<RenderUnit*>());
		}
		pbr_mat_unit_map[pbr_mat].emplace(ru);
	}

}

void RenderManager::BindSkyboxTexture(HDRTextureFile* hdr_file)
{
	if (!b_skybox_initialized)
	{
		skybox_cubemap = dynamic_cast<CubeMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::CUBEMAP));
		skybox_cubemap->width = 1024;
		skybox_cubemap->height = 1024;
		skybox_cubemap->b_genarate_mipmap = false;
		skybox_cubemap->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;
		skybox_cubemap->Buffer();
	}

	//HDRTextureFile* hdr_file = dynamic_cast<HDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("hdr/old_hall.hdr"), TextureFileType::HDR));
	EquirectangularMap* equirectanguler_map = dynamic_cast<EquirectangularMap*>(ResourceManager::GetSingleton().CreateTexture(TextureType::EQUIRECTANGULARMAP, hdr_file, true));	//? 先删除旧的
	//equirectanguler_map->Buffer();

	HDRIShader hdri_cubemap_shader;
	hdri_cubemap_shader.RenderCubeMap(skybox_cubemap, equirectanguler_map->texture_id);

}


