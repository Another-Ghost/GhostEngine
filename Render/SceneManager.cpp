#include "SceneManager.h"
#include "RenderManager.h"
#include "RenderNode.h"
#include "RenderModule.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderManager.h"
#include "CameraFactory.h"
#include "LightFactory.h"
#include "ReflectionProbe.h"
#include "PointLight.h"

template<> SceneManager* Singleton<SceneManager>::singleton = nullptr;
SceneManager::SceneManager()
{
	root_unit = new Node();
}
void SceneManager::AddRenderNode(RenderNode* unit, Node* parent)
{
	if (!parent)
		parent = root_unit;
	unit->SetParent(parent);
	parent->AddChild(unit);
}

Camera* SceneManager::CreateCamera(const CameraFactory& camera_factory)	//? 需改为传入枚举类型
{
	Camera* camera = camera_factory.CreateCamera();
	camera_array.emplace_back(camera);
	if (!main_camera)
	{
		main_camera = camera;
	}
	return camera;
}

void SceneManager::BindCamera(Camera* camera_)
{
	camera_array.push_back(camera_);
	main_camera = camera_;
}

void SceneManager::Update(float dt)
{
	main_camera->Update(dt);

	root_unit->Update(dt);

	//for (const auto& render_unit : render_unit_array)	//可以改成保存unit数组，dynamic_cast判断是否是render_unit
	//{
	//	RenderModule* render_module = render_unit->render_module;
	//	if (render_module)
	//	{
	//		Material* material = render_module->material;
	//		//Mesh* mesh = render_module->mesh;
	//		if (material)
	//		{
	//			RenderManager::GetSingleton().pbr_mat_module_map.emplace(material, render_module);
	//		}
	//	}
	//}
}

void SceneManager::AddPointLight(PointLight* light)
{
	point_light_array.emplace_back(light);
}

Light* SceneManager::CreateLight(const LightFactory& light_factory)
{
	PointLight* light = dynamic_cast<PointLight*>(light_factory.CreateLight());
	
	
	point_light_array.emplace_back(light);

	//RenderManager::GetSingleton().UpdateLightArray();

	return light;
}

ReflectionProbe* SceneManager::CreateReflectionProbe(const vec3& position, const AABBModule& aabb_module)
{
	ReflectionProbe* reflection_probe = new ReflectionProbe(position, aabb_module);

	reflection_probe_set.emplace(reflection_probe);

	return reflection_probe;
}



