#include "SceneManager.h"
#include "RenderManager.h"
#include "RenderUnit.h"
#include "RenderModule.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderManager.h"

template<> SceneManager* Singleton<SceneManager>::singleton = nullptr;
SceneManager::SceneManager()
{
	root_unit = new Unit();
}
void SceneManager::AddRenderUnit(RenderUnit* unit, Unit* parent)
{
	if (!parent)
		parent = root_unit;
	unit->SetParent(parent);
	parent->AddChild(unit);
}

void SceneManager::Update(float dt)
{
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

void SceneManager::AddLight(Light* light)
{
	light_array.emplace_back(light);
}



