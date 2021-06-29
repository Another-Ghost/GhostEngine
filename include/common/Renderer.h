#pragma once
#include "Prerequisite.h"
#include "Window.h"
#include "Shader.h"


class Renderer : public WindowEventListener
{
	Shader* shader;

	virtual void Update(float dt) = 0;

	friend class RenderManager;
};

