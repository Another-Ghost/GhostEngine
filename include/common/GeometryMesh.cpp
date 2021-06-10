#include "GeometryMesh.h"

GeometryMesh::GeometryMesh()
{

}

void GeometryMesh::Initialize()
{
}

void GeometryMesh::Buffer()
{
	// create buffers/arrays
	glGenVertexArrays(1, &vao_id);
	glGenBuffers(1, &vbo_id);
	glGenBuffers(1, &ebo_id);

	glBindVertexArray(vao_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, vertex_array.size() * sizeof(Vertex), &vertex_array[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_array.size() * sizeof(unsigned int), &index_array[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //启用编号0的顶点属性，0对应于shader中的 layout(location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glBindVertexArray(0);
}

void GeometryMesh::Draw()
{
	glBindVertexArray(vao_id);
	glDrawElements(GL_TRIANGLE_STRIP, index_array.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(vao_id);
}
