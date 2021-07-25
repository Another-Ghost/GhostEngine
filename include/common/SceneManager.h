#pragma once
#include "Prerequisite.h"
#include "Singleton.h"


class SceneManager : public Singleton<SceneManager>
{
public:
	vector<Camera*> camera_array;

	vector<RenderNode*> render_unit_array;

	vector<Light*> light_array;

	Camera* main_camera;

	Node* root_unit;

	SceneManager();

	void AddRenderNode(RenderNode* unit, Node* parent = nullptr);

	Camera* CreateCamera(const CameraFactory& camera_factory);

	void BindCamera(Camera* camera_);

	void Update(float dt);

	Light* CreateLight(const LightFactory& light_factory);
	//Node* CreateNode();

private:
	void AddLight(Light* light);
};

