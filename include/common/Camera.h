#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

enum class CameraCommand
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{

public:


	//const float kFOVY = 90.0f;
	//const float kAspect = 16 / 9.0f;
	//const float kNearPlane = 0.1f;
	//const float kFarPlane = 100.0f;
	//const float kMoveSpeed = 2.5f;
	//const float kRotationSpeed = 0.1f;

	Camera(const Transform& tf = Transform(), float aspect_ = 16 / 9.0f, float fov_y_ = 45.f, float near = 0.1f, float far = 100.f, float move_speed_ = 2.5f, float roation_speed_ = 0.2f)
	:transform(tf), aspect(aspect_), fov_y(glm::radians(fov_y_)), near_plane(near), far_plane(far), move_speed(move_speed_), rotation_speed(roation_speed_){ }

	virtual ~Camera();

	glm::mat4 ViewMatrix();

	glm::mat4 PerspectiveMatrix();

	//void SetTransform(const Transform& tf) { transform = tf; }
	//Transform GetTransform() { return transform; }

	float GetRotationSpeed() { return rotation_speed; }
	void SetRotationSpeed(float rotation_spead_) { /*add verification*/ rotation_speed = rotation_spead_; }

	float GetMoveSpeed();

	float GetPitch() { return pitch; }
	void SetPitch(float pitch_);
	float GetYaw(){ return yaw; }
	void SetYaw(float yaw_);

	void SetEulerAngle(const vec3& euler);

	vec3 GetPosition() { return transform.GetPosition(); }
	void SetPosition(const vec3& position_) { transform.SetPosition(position_); }

	vec3 GetForward() { return transform.GetForward(); }

	vec3 GetRight() { return transform.GetRight(); }

protected:	//? effective c++

	float pitch;
	float yaw;

	Transform transform;

	float near_plane;
	float far_plane;
	//float left;
	//float right;
	//float top;
	//float bottom;

	float fov_y;	//radians
	float aspect;

	float move_speed;
	float rotation_speed;

	//parent

};



