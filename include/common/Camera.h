#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

enum class CameraControl
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
protected:	//? effective c++

	float near_plane;
	float far_plane;
	//float left;
	//float right;
	//float top;
	//float bottom;

	float fov_y;
	float aspect;

	float move_speed;
	float rotation_spead;


public:
	Transform transform;

	//const float kFOVY = 90.0f;
	//const float kAspect = 16 / 9.0f;
	//const float kNearPlane = 0.1f;
	//const float kFarPlane = 100.0f;
	//const float kMoveSpeed = 2.5f;
	//const float kRotationSpeed = 0.1f;

	Camera(const Transform& tf = Transform(), float aspect_ = 16 / 9.0f, float fov_y_ = 90.f, float near = 0.1f, float far = 100.f, float move_speed_ = 2.5f, float roation_speed_ = 0.1f)
	:transform(tf), aspect(aspect_), fov_y(fov_y_), near_plane(near), far_plane(far), move_speed(move_speed_), rotation_spead(roation_speed_){ }

	void ProcessKeyboard(CameraControl type, float dt);

	void ProcessMouseMovement(float x_offset, float y_offset);
 
	void ProcessMouseScroll(float y_offset);

	glm::mat4 ViewMatrix();

	glm::mat4 PerspectiveMatrix();
};



