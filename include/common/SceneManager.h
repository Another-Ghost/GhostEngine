#pragma once
#include "Prerequisite.h"
#include "Singleton.h"


class SceneManager : public Singleton<SceneManager>
{
public:
	vector<Camera*> camera_array;

	vector<RenderUnit*> render_unit_array;

	vector<Light*> light_array;

	Camera* main_camera;

	Unit* root_unit;

	SceneManager();

	void AddRenderUnit(RenderUnit* unit, Unit* parent = nullptr);

	Camera* CreateCamera(const CameraFactory& camera_factory);

	void BindCamera(Camera* camera_);

	void Update(float dt);
	
	void DrawModule(RenderModule* module);

	void AddLight(Light* light);
	//Unit* CreateUnit();
};

