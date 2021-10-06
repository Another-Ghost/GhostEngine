#pragma once
#include "Prerequisite.h"
#include "Singleton.h"

//include all objects with postion

class SceneManager : public Singleton<SceneManager>
{
public:
	vector<Camera*> camera_array;

	vector<RenderNode*> render_unit_array;

	vector<PointLight*> point_light_array;

	Camera* main_camera;

	Node* root_node;

	set<ReflectionProbe*> reflection_probe_set;

	SceneManager();

	void AddRenderNode(RenderNode* unit, Node* parent = nullptr);

	Camera* CreateCamera(const CameraFactory& camera_factory);

	void BindCamera(Camera* camera_);

	void Update(float dt);

	Light* CreateLight(const LightFactory& light_factory);
	//Node* CreateNode();
	ReflectionProbe* CreateReflectionProbe(const vec3& position, const AABBModule& aabb_module);

private:
	void AddPointLight(PointLight* light);
};

