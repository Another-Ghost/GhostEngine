#pragma once
#include "Mesh.h"

struct Vertex
{
	vec3 position;
	vec2 tex_coord;
	vec3 normal;
};

class GeometryMesh : public Mesh
{
protected:
	vector<Vertex> vertex_array;
	vector<unsigned int> index_array;
};

