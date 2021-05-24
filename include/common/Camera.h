#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"


class Camera
{
protected:	//? effective c++

	float near_plane;
	float far_plane;
	float left;
	float right;
	float top;
	float bottom;

	float fov_y;
	float aspect;

	float move_speed;
	float mouse_spead;


public:
	Transform transform;

	void ProcessMouseMovement(float x_offset, float y_offset);
 
	void ProcessMouseScroll(float y_offset);

	glm::mat4 ViewMatrix();

	glm::mat4 PerspectiveMatrix();
};

