#include "Transform.h"
#include <iostream>


void Transform::UpdateMatrix()
{
	/*mat4 pos = translate(mat4(1), position);
	mat4 ori = rotate(mat4(1), angle(orientation), axis(orientation));
	mat4 sca = scale(mat4(1), dimension);
	mat4 identity = mat4(1);
	matrix = translate(mat4(1), position) * rotate(mat4(1), angle(orientation), axis(orientation)) * scale(mat4(1), dimension);*/
	
	matrix = translate(mat4(1), position) * mat4_cast(orientation) * scale(mat4(1), dimension);

	//matrix = mat4_cast(orientation);
}

Transform::Transform(const vec3& pos, const quat& ori, const vec3& dim)
{
	position = pos;
	orientation = ori;
	dimension = dim;
	UpdateMatrix();
}

Transform::Transform(const mat4& matrix_)
{
	matrix = matrix_; //? （正交化）解决矩阵蠕变
	dimension = vec3(vec3(matrix[0]).length(), vec3(matrix[1]).length(), vec3(matrix[2]).length());
	orientation = quat(matrix);
	position = vec3(matrix[0][3], matrix[1][3], matrix[2][3]) / dimension;
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

vec3 Transform::GetForward()
{
	//return normalize(vec3(matrix[0][2], matrix[1][2], matrix[2][2]));
	return normalize(-vec3(matrix[2]));

}

vec3 Transform::GetRight()
{
	//return normalize(vec3(matrix[0][0], matrix[1][0], matrix[2][0]));
	return normalize(vec3(matrix[0]));

}

glm::vec3 Transform::GetUpword()
{
	//return normalize(vec3(matrix[0][1], matrix[1][1], matrix[2][1]));
	return normalize(vec3(matrix[1]));
}

const Transform& Transform::Translate(vec3 vector)
{
	SetPosition(position + vector);
	return *this;
}

const Transform& Transform::Scale(vec3 scale_)
{
	SetDimension(dimension * scale_);
	return *this;
}

Transform Transform::operator*(const Transform& other)
{
	return Transform(matrix * other.matrix);
}

Transform Transform::operator*=(const Transform& other)
{
	*this = Transform(matrix * other.matrix);
	return *this;
}

void Transform::PrintMatrix()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}



