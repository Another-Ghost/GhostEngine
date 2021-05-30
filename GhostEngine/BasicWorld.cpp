#include "BasicWorld.h"
#include "BasicRepositry.h"
#include "BasicRenderer.h"
#include "common/WindowInitializer.h"

Window* WindowInitializer::window = nullptr;

BasicWorld::BasicWorld()
{

	window = WindowInitializer().CreateWindow(1280, 720);
	camera = new Camera(Transform(vec3(0.f, 0.f, 3.f)));
	window->SetCamera(camera);
	renderer = new BasicRenderer(this, window);
}

void BasicWorld::Update()
{
	while (window->Update(delta_time)) //? 拆成两步
	{
		renderer->Update(delta_time);
		window->EndUpdate();
	}

	glfwTerminate();
}

