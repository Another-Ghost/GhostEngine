#include "Root.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "RenderManager.h"

template<> Root* Singleton<Root>::singleton = nullptr;
Root::Root() :
max_delta_time(1/60.f)
{
	s_window_manager = std::make_unique<WindowManager>();
	s_scene_manager = std::make_unique<SceneManager>();
	s_resource_manager = std::make_unique<ResourceManager>();
	s_render_manager = std::make_unique<RenderManager>();
}

void Root::Loop()
{
	Window* window = WindowManager::GetSingleton().current_window;
	delta_time = max_delta_time;
	if (!window->WindowShouldClose())
	{
		Update(delta_time);

		float current_frame_time = window->GetCurrentFrameTime();
		delta_time = current_frame_time - last_frame_time;
		last_frame_time = current_frame_time;
	}
	
}

void Root::Update(float dt)
{
	WindowManager::GetSingleton().Update(dt);
	SceneManager::GetSingleton().Update(dt);

	WindowManager::GetSingleton().EndUpdate(dt);
}

void Root::RenderOneFrame(float dt)
{

}

void Root::Terminate()
{
	WindowManager::GetSingleton().Terminate();
}
