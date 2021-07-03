#pragma once
#include "Prerequisite.h"
#include "Window.h"
#include "Shader.h"
#include "File.h"

class Renderer : public WindowEventListener
{
	//Shader* shader;
public:
	virtual void Update(float dt) = 0;

	//friend class RenderManager;
};

