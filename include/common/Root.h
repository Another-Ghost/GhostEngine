#pragma once
#include "Prerequisite.h"
#include "Singleton.h"

class Root : public Singleton<Root>
{
public:
	unique_ptr<WindowManager> s_window_manager;
	unique_ptr<ResourceManager> s_resource_manager;
	unique_ptr<SceneManager> s_scene_manager;
	unique_ptr<RenderManager> s_render_manager;

	Root();

	bool Initialize();

	void Loop();

	void Update(float dt);

	void RenderOneFrame(float dt);

	void Terminate();

private:
	float delta_time;
	float last_frame_time;
	float max_delta_time;
};

