#include "Transform.h"



void Transform::UpdateMatrix()
{
	matrix = translate(mat4(1), position) * rotate(mat4(1), angle(orientation), axis(orientation)) * scale(mat4(1), dimension);
}

Transform::Transform(const vec3& pos, const quat& ori, const vec3 dim)
{
	position = pos;
	orientation = pos;
	dimension = dim;
	UpdateMatrix();
}

void Transform::SetPosition(const vec3& pos)
{
	position = pos;
	UpdateMatrix();
}

void Transform::SetOrientation(const quat ori)
{
	orientation = ori;
	UpdateMatrix();
}

void Transform::SetDimension(const vec3& dim)
{
	dimension = dim;
	UpdateMatrix();
}

