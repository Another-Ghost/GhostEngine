#pragma once
#include "Prerequisite.h"
#include "Singleton.h"
class RenderManager : public Singleton<RenderManager>
{
	RenderManager();

	Shader* pbr_shader;

	Shader* current_shader;
};

