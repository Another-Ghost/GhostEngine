#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

using namespace glm;

class Camera
{
private:
	Transform transform;

public:
	void ProcessMouseMovement(float x_offset, float y_offset);
 
	void ProcessMouseScroll(float y_offset);
};

