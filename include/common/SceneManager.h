#pragma once
#include "Prerequisite.h"
#include "Singleton.h"


class SceneManager : public Singleton<SceneManager>
{
public:
	vector<Camera*> camera_array;

	vector<RenderUnit*> render_unit_array;

	vector<Light*> light_array;

	Camera* CreateDefaultCamera();

	void BindCamera(Camera* camera_) { camera_array.push_back(camera_); }

	void Update(float dt);
	
	void DrawModule(RenderModule* module);

	//Unit* CreateUnit();
};

