#pragma once
#include "Prerequisite.h"
#include "Shader.h"

class Renderer
{
	Shader* shader;

	virtual void Update(float dt) = 0;

	friend class RenderManager;
};

