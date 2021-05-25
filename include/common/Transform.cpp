#include "Transform.h"



void Transform::UpdateMatrix()
{
	matrix = translate(mat4(1), position) * rotate(mat4(1), angle(orientation), axis(orientation)) * scale(mat4(1), dimension);
	//matrix = mat4_cast(orientation);
}

Transform::Transform(const vec3& pos, const quat& ori, const vec3& dim)
{
	position = pos;
	orientation = ori;
	dimension = dim;
	UpdateMatrix();
}

void Transform::SetPosition(const vec3& pos)
{
	position = pos;
	UpdateMatrix();
}

void Transform::SetOrientation(const quat& ori)
{
	orientation = ori;
	UpdateMatrix();
}

void Transform::SetDimension(const vec3& dim)
{
	dimension = dim;
	UpdateMatrix();
}


void Transform::SetPitch(float value)
{
	glm::vec3 pre_euler = GetEulerAngle();
	SetOrientation(vec3(glm::radians(value), pre_euler.y, pre_euler.z));
}

void Transform::SetYaw(float value)
{
	glm::vec3 pre_euler = GetEulerAngle();
	SetOrientation(vec3(pre_euler.x, glm::radians(value), pre_euler.z));
}

void Transform::SetRoll(float value)
{
	glm::vec3 pre_euler = GetEulerAngle();
	SetOrientation(vec3(pre_euler.x, pre_euler.y, glm::radians(value)));
}

glm::vec3 Transform::GetForward()
{
	return normalize(vec3(matrix[0][0], matrix[1][0], matrix[2][0]));
}

glm::vec3 Transform::GetRight()
{
	return normalize(vec3(matrix[0][1], matrix[1][1], matrix[2][1]));
}

glm::vec3 Transform::GetUpword()
{
	return normalize(vec3(matrix[0][2], matrix[1][2], matrix[2][2]));
}



