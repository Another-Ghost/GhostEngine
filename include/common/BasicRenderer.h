#pragma once
#include "Renderer.h"
class BasicRenderer : public Renderer
{
public:
	BasicRenderer();

	void Update(float dt) override;


private:
	BasicShader* basic_shader;
};

