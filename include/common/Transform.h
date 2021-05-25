#pragma once
//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::mat4;
using glm::vec3;
using glm::quat;

struct Transform
{
private:
	mat4 matrix;
	vec3 position;
	quat orientation;
	vec3 dimension;

	void UpdateMatrix();

public:
	Transform(const glm::vec3& pos = { 0, 0, 0 }, const quat& ori = {1, 0, 0, 0}, const vec3& dim = { 1, 1, 1 });

	void SetPosition(const vec3& pos);

	void SetOrientation(const quat& ori);

	void SetDimension(const vec3& sca);

	vec3 GetPosition() { return position; }

	quat GetOrientation() { return orientation; }

	vec3 GetDimention() { return dimension; }

	vec3 GetEulerAngle() { return eulerAngles(orientation); }

	float GetPitch() { return glm::degrees(pitch(orientation)); }	//? 用不用inline，inline的判断到底是语句数，还是包括调用的函数的代码量

	float GetYaw() { return glm::degrees(yaw(orientation)); }

	float GetRoll() { return glm::degrees(roll(orientation)); }

	void SetEulerAngle(const vec3& euler_angle) { SetOrientation(quat(radians(euler_angle))); }//如果同时设置多个，最好用此函数

	void SetPitch(float value);
	
	void SetYaw(float value);

	void SetRoll(float value);

	vec3 GetForward();

	vec3 GetRight();

	vec3 GetUpword();

};

