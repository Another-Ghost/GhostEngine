#pragma once
#include "common/Renderer.h"


class BasicRenderer : public Renderer
{
private:
	BasicRenderer();

	bool Init() override;

	bool Update(float& dt);


private:
	Shader* basic_shader;

	Window* window;
};

