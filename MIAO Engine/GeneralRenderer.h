#pragma once
#include "common/Renderer.h"

class GeneralRenderer : public Renderer
{
private:
	bool Init() override;

	void Update();
	
	Shader* basic_shader;


};

