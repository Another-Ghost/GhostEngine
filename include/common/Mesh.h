#pragma once
#include "Prerequisite.h"
#include "Shader.h"
#include "AABBVolume.h"

enum class MeshType
{
	GEOMETRY,
	TRIANGLE
};

struct Vertex
{
	vec3 position;
	vec2 tex_coords;
	vec3 normal;
};

class Mesh
{
public:
	vector<Mesh*> child_mesh_array;

	bool b_buffered;	//是否绑定缓存
	bool b_Render;	//是否渲染

	unsigned int vao_id;

	vector<Vertex> vertex_array;
	vector<unsigned int> index_array;

	//protected:

	unsigned int vbo_id;
	unsigned int ebo_id;

	AABBVolume aabb_volume;

	//Transform offset_transform;

	//Mesh(const Transform& offset_transform_ = Transform());
	virtual void Initialize();	//Template Method


	virtual void Buffer();

	virtual void Draw(Shader* shader) = 0;	//输入shader是为了确保shader.use()的调用
	//virtual void Draw() = 0;

	virtual void InitializeAABB() = 0;
};


