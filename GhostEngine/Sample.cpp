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
#include "common/RenderModule.h"
#include "common/SphereGeometryMesh.h"
#include "common/SceneManager.h"
#include <memory>

int main()
{
	//Init
	Root* root =  new Root();
	
	BasicWindowFactory* window_factory = new BasicWindowFactory();

	BasicWindow* window = dynamic_cast<BasicWindow*>(WindowManager::GetSingleton().CreateWindow(window_factory, 1280, 720, "Demo"));

	Root::GetSingleton().Initialize();

	BasicCamera* camera = new BasicCamera();

	window->SetCamera(camera);
	SceneManager::GetSingleton().BindCamera(camera);

	BasicUnit* sphere_unit = new BasicUnit();

	RenderModule* render_module = new RenderModule();

	PBRMaterial* material = dynamic_cast<PBRMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::PBR));
	material->specular_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/rusted_iron/albedo.png"), TextureType::SPECULAR);
	material->roughness_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/rusted_iron/roughness.png"), TextureType::ROUGHNESS);
	material->normal_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/rusted_iron/normal.png"), TextureType::NORMAL);
	material->metalness_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/rusted_iron/metallic.png"), TextureType::METALNESS);
	material->ao_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("pbr/rusted_iron/ao.png"), TextureType::AO);
	
	render_module->material = material;
	
	SphereGeometryMesh* mesh = new SphereGeometryMesh(10.f); //? should be created be the resource manager
	render_module->mesh = mesh;

	sphere_unit->AttachRenderModule(render_module);
	SceneManager::GetSingleton().AddRenderUnit(sphere_unit);

	PointLight* light = new PointLight();
	SceneManager::GetSingleton().AddLight(light);

	

	//Shader* shader = new Shader("");
	//Loop
	float current_frame_time;
	float delta_time = 1/60.f;
	float last_frame_time = 0.f;
	while (!window->WindowShouldClose())
	{
		root->Update(delta_time);

		float current_frame_time = window->GetCurrentFrameTime();

		delta_time = current_frame_time - last_frame_time;
		last_frame_time = current_frame_time;
	}

	Root::GetSingleton().Terminate();

	return 0;
}