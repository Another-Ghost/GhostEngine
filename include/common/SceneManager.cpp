#include "SceneManager.h"
#include "RenderManager.h"
#include "RenderUnit.h"
#include "RenderModule.h"
#include "GeometryMesh.h"
#include "PBRMaterial.h"

void SceneManager::Update(float dt)
{
	for (const auto& render_unit : render_unit_array)	//可以改成保存unit数组，dynamic_cast判断是否是render_unit
	{
		RenderModule* render_module = render_unit->render_module;
		if (render_module)
		{
			Material* material = render_module->material;
			Mesh* mesh = render_module->mesh;
			if (dynamic_cast<PBRMaterial*>(material))
			{
				
			}

			if (dynamic_cast<GeometryMesh*>(render_module->mesh))
			{
				GeometryMesh* geometry_mesh = dynamic_cast<GeometryMesh*>(render_module->mesh);
				
			}
		}
	}
}

void SceneManager::DrawModule(RenderModule* module)
{
	Material* material = module->material;
	Mesh* mesh = module->mesh;
	if (dynamic_cast<PBRMaterial*>(material))
	{
		
	}


}


