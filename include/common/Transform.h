#pragma once
//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Transform
{
private:
	mat4 matrix;
	vec3 position;
	quat orientation;
	vec3 dimension;

	void UpdateMatrix();

public:
	Transform(const vec3& pos = { 0, 0, 0 }, const quat& ori = {1, 0, 0, 0}, const vec3 dim = { 1, 1, 1 });

	void SetPosition(const vec3& pos);

	void SetOrientation(const quat ori);

	void SetDimension(const vec3& sca);

	vec3 GetPosition() { return position; }

	quat GetOrientation() { return orientation; }

	vec3 GetDimention() { return dimension; }

	float GetPitch();

	float GetYaw();

	float GetRoll();
};

