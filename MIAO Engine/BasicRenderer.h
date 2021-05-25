#pragma once
#include "common/Renderer.h"


class BasicRenderer : public Renderer
{
public:
	BasicRenderer(World* world_, Window* window_) :Renderer(world_, window_) {
		Initialize();
		BeginRender();
	}

	bool Initialize() override;

	void BeginRender() override;

	void Update(float dt) override;



private:
	Shader* basic_shader;

	Window* window;

	Camera* camera;

	World* world;
};

