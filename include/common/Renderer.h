#pragma once
#include "WindowInitializer.h"
#include "window.h"
#include "Shader.h"
#include "Scene.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class World;


class Renderer
{
public:
	Renderer(World* world_, Window* window_) : world(world_), window(window_) { }

	virtual void Update(float dt) = 0;

protected:

	virtual bool Initialize() = 0;

	virtual void BeginRender() = 0;

	World* world;

	Window* window;

};

