#pragma once
#include "Prerequisite.h"
#include "Transform.h"
#include <glad/glad.h> // holds all OpenGL type declarations



class Mesh
{
public:

	Transform offset_transform;

	//Mesh(const Transform& offset_transform_ = Transform());

	virtual void Buffer();

	virtual void Draw();

	vector<Mesh*> child_mesh_array;

	bool b_buffered;	//是否绑定缓存
	bool b_Render;	//是否渲染

	unsigned int VAO;
};


