#pragma once
#include "Prerequisite.h"

class Root
{
public:
	unique_ptr<WindowManager> s_window_manager;
	unique_ptr<ResourceManager> s_resource_manager;
	unique_ptr<SceneManager> s_scene_manager;

	Root();

	void Loop();

	void Update(float dt);

	void RenderOneFrame(float dt);

private:
	float delta_time;
	float last_frame_time;
	float max_delta_time;
};

