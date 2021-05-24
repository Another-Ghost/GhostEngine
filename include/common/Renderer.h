#pragma once
#include "WindowInitializer.h"
#include "window.h"
#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Renderer
{
protected:

	virtual bool Init() = 0;

};

