#include "common/Root.h"
#include "common/WindowManager.h"
#include "BasicWindowFactory.h"
#include "BasicWindow.h"
#include "BasicCamera.h"
#include "BasicUnit.h"
#include "common/RenderModule.h"
#include "common/ResourceManager.h"
#include "common/PBRMaterial.h"
#include "common/Texture.h"
#include "common/File.h"
#include "common/PointLight.h"
#include "common/RootRenderModule.h"
#include "common/SphereGeometryMesh.h"
#include "common/SceneManager.h"
#include "common/CameraFactory.h"
#include "common/SphereGeometryMeshFactory.h"
#include "common/PointLightFactory.h"
#include "common/TextureFile.h"
#include <memory>

//#define SAMPLE_IBL
#ifdef SAMPLE_IBL

int main()
{
	//Init
	Root* root = new Root(1280, 720, "Demo");
	BasicWindowFactory* window_factory = new BasicWindowFactory();
	//BasicWindow* window = dynamic_cast<BasicWindow*>(WindowManager::GetSingleton().CreateWindow(window_factory, 1280, 720, "Demo"));
	Root::GetSingleton().Initialize();

	Camera* camera = SceneManager::GetSingleton().CreateCamera(CameraFactory());
	camera->SetPosition(vec3(0, 0, 3.5));
	Window* window = WindowManager::GetSingleton().current_window;
	//window->SetCamera(camera);	//? 改成在内部initialize里从SceneManger获取
	window->AddEventListener(camera);

	//add unit
	BasicUnit* sphere_unit = new BasicUnit();

	RootRenderModule* root_render_module = new RootRenderModule();

	PBRMaterial* material = dynamic_cast<PBRMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::PBR));
	LDRTextureFile* albedo_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("pbr/painted_metal/albedo.png"), TextureFileType::LDR));
	material->albedo_map = ResourceManager::GetSingleton().CreateTexture(TextureType::ALBEDO, albedo_file);
	material->albedo_map->Buffer();
	LDRTextureFile* normal_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("pbr/painted_metal/normal.png"), TextureFileType::LDR));
	material->normal_map = ResourceManager::GetSingleton().CreateTexture(TextureType::NORMAL, normal_file);
	material->normal_map->Buffer();
	LDRTextureFile* metalness_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("pbr/painted_metal/metallic.png"), TextureFileType::LDR));
	material->metalness_map = ResourceManager::GetSingleton().CreateTexture(TextureType::METALNESS, metalness_file);
	material->metalness_map->Buffer();
	LDRTextureFile* roughness_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("pbr/painted_metal/roughness.png"), TextureFileType::LDR));
	material->roughness_map = ResourceManager::GetSingleton().CreateTexture(TextureType::ROUGHNESS, roughness_file);
	material->roughness_map->Buffer();
	LDRTextureFile* ao_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(File::GetTexturePath("pbr/painted_metal/ao.png"), TextureFileType::LDR));
	material->ao_map = ResourceManager::GetSingleton().CreateTexture(TextureType::AO, ao_file);
	material->ao_map->Buffer();

	root_render_module->SetMaterial(material);

	SphereGeometryMesh* mesh = dynamic_cast<SphereGeometryMesh*>(ResourceManager::GetSingleton().CreateMesh(SphereGeometryMeshFactory())); //? should be created by the resource manager
	root_render_module->SetMesh(mesh);
	sphere_unit->AttachRenderModule(root_render_module);

	//add light
	PointLight* light = dynamic_cast<PointLight*>(SceneManager::GetSingleton().CreateLight(PointLightFactory()));
	light->postion = vec3(0.f, 0.f, 10.f);

	//Loop
	float max_delta_time = 1.f / 60.f;
	float current_frame_time;
	float delta_time = 1 / 60.f;
	float last_frame_time = 0.f;
	while (!window->WindowShouldClose())
	{
		root->Update(delta_time);

		current_frame_time = window->GetCurrentFrameTime();

		delta_time = std::min(current_frame_time - last_frame_time, max_delta_time);
		last_frame_time = current_frame_time;
	}
	Root::GetSingleton().Terminate();

	return 0;
}

#endif