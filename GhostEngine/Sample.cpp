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
#include <memory>

int main()
{
	//Init
	Root* root =  new Root();
	
	BasicWindowFactory* window_factory = new BasicWindowFactory();

	BasicWindow* window = dynamic_cast<BasicWindow*>(WindowManager::GetSingleton().CreateWindow(window_factory, 1280, 720, "Demo"));

	BasicCamera* camera = new BasicCamera();

	window->SetCamera(camera);

	BasicUnit* sphere_unit = new BasicUnit();
	PBRMaterial* material = dynamic_cast<PBRMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::PBR));
	material->specular_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("/pbr/rusted_iron/albedo.png"), TextureType::SPECULAR);
	material->roughness_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("/pbr/rusted_iron/roughness.png"), TextureType::ROUGHNESS);
	material->specular_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("/pbr/rusted_iron/normal.png"), TextureType::NORMAL);
	material->specular_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("/pbr/rusted_iron/metallic.png"), TextureType::METALNESS);
	material->specular_map = ResourceManager::GetSingleton().CreateTexture(File::GetTexturePath("/pbr/rusted_iron/ao.png"), TextureType::AO);
	sphere_unit->render_module->material = material;

	PointLight* light = new PointLight();

	//Shader* shader = new Shader("");
	//Loop
	float current_frame_time;
	float delta_time = 1/60.f;
	float last_frame_time;
	if (!window->WindowClosinng())
	{
		root->Update(delta_time);
		
		float current_frame_time = window->GetCurrentFrameTime();
	
		delta_time = current_frame_time - last_frame_time;
		last_frame_time = current_frame_time;
	}

	return 0;
}