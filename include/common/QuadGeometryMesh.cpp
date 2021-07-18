#include "QuadGeometryMesh.h"

QuadGeometryMesh::QuadGeometryMesh()
{
	Initialize();

	if (!b_buffered)
	{
		Buffer();
	}
}

void QuadGeometryMesh::Initialize()
{
	vertex_array = {
		// positions        // texture Coords
		{{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}, {0.f, 0.f, 1.f}},
		{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.f, 0.f, 1.f}},
		{{1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}, {0.f, 0.f, 1.f}},
		{{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {0.f, 0.f, 1.f}}
	};

	index_array = {
		0, 1, 3,
		1, 2, 3
	};
}

void QuadGeometryMesh::Draw(Shader* shader)
{
	shader->Use();

	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
