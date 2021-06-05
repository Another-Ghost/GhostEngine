﻿#pragma once
#include "Prerequisite.h"
#include "Transform.h"
#include <glad/glad.h> // holds all OpenGL type declarations

struct Vertex
{
	vec3 position;
	vec2 tex_coords;
	vec3 normal;
};

class Mesh
{
public:

	//Transform offset_transform;

	//Mesh(const Transform& offset_transform_ = Transform());

	virtual void Buffer();

	//virtual void Draw() = 0;

	vector<Mesh*> child_mesh_array;

	bool b_buffered;	//是否绑定缓存
	bool b_Render;	//是否渲染

	unsigned int vao_id;

	vector<Vertex> vertex_array;
	vector<unsigned int> index_array;
};


