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

	Root(int win_width, int win_height, const string& win_title);

	bool Initialize(Renderer * renderer = nullptr);

	void Loop();

	void Update(float dt);

	void RenderOneFrame(float dt);

	void Terminate();

	float GetDeltaTime() { return delta_time; }

private:
	float delta_time;
};

