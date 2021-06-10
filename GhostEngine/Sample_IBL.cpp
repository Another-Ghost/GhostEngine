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
#include "common/HDRTexture.h"
#include <memory>



int main()
{
	//Init
	Root* root = new Root();
	BasicWindowFactory* window_factory = new BasicWindowFactory();
	BasicWindow* window = dynamic_cast<BasicWindow*>(WindowManager::GetSingleton().CreateWindow(window_factory, 1280, 720, "Demo"));
	Root::GetSingleton().Initialize();

	Camera* camera = SceneManager::GetSingleton().CreateCamera(CameraFactory());
	camera->SetPosition(vec3(0, 0, 3));
	window->SetCamera(camera);	//? 改成在内部initialize里从SceneManger获取

	//add unit
	BasicUnit* sphere_unit = new BasicUnit();

	RootRenderModule* root_render_module = new RootRenderModule();

	PBRMaterial* material = dynamic_cast<PBRMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::PBR));
	material->albedo_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/plastic/albedo.png"), TextureType::SPECULAR);
	material->normal_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/plastic/normal.png"), TextureType::NORMAL);
	material->metalness_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/plastic/metallic.png"), TextureType::METALNESS);
	material->roughness_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/plastic/roughness.png"), TextureType::ROUGHNESS);
	material->ao_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/plastic/ao.png"), TextureType::AO);
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

		float current_frame_time = window->GetCurrentFrameTime();

		delta_time = std::min(current_frame_time - last_frame_time, max_delta_time);
		last_frame_time = current_frame_time;
	}
	Root::GetSingleton().Terminate();

	return 0;
}
