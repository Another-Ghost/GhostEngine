#pragma once
#include "Prerequisite.h"
#include "Singleton.h"
#include "RenderModule.h"

struct RenderMaterialComp
{
	bool operator()(RenderModule* a, RenderModule* b)
	{
		Unit* parent_a = a->GetParent();
		Unit* parent_b = b->GetParent();
		if (parent_a && parent_b)
		{
			return parent_a->transform.GetPosition().z > parent_b->transform.GetPosition().z;
		}
		else
		{
			cout << "ERROR<RenderManager> from MaterialComp: parent is null \n";
			return false;
		}
	}
};

class RenderManager : public Singleton<RenderManager>
{
public:
	RenderManager();
	
	void Update(float dt);

	void RenderPBRMaterial(float dt);

	void ResetRenderArray();

private:
	Shader* pbr_shader;

	Shader* current_shader;

	//map<Material*, set<RenderModule*>> mat_module_map;
	map<PBRMaterial*, set<RenderModule*>> pbr_mat_module_map;
	
	//set<RenderModule*> set_map;
	//render_module_group

	friend class SceneManager;

	mat4 model_matrix;
	mat4 view_matrix;
	mat4 perspective_matrix;

	Camera* camera;

	friend class RenderUnit;
};

