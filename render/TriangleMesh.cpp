#include "TriangleMesh.h"

void TriangleMesh::Initialize()
{
	InitializeAABB();
	if (!b_buffered)
	{
		Buffer();
	}
}

void TriangleMesh::Buffer()
{
	// create buffers/arrays
	glGenVertexArrays(1, &vao_id);
	glGenBuffers(1, &vbo_id);
	glGenBuffers(1, &ebo_id);

	glBindVertexArray(vao_id);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertex_array.size() * sizeof(ExpandedVertex), &vertex_array[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_array.size() * sizeof(unsigned int), &index_array[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0); //参数对应于shader中的position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)0);	//? 改成连续存储
	// vertex texture coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)offsetof(ExpandedVertex, tex_coords));
	// vertex normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)offsetof(ExpandedVertex, normal));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)offsetof(ExpandedVertex, tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)offsetof(ExpandedVertex, bitangent));

	glBindVertexArray(0);
}

void TriangleMesh::Draw(Shader* shader)
{
	shader->Use();
	// draw mesh
	glBindVertexArray(vao_id);
	glDrawElements(GL_TRIANGLES, index_array.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void TriangleMesh::InitializeAABB()
{
	vec3 max_half_size{0.f, 0.f, 0.f};
	for (const auto& vertex : vertex_array)
	{
		if (abs(vertex.position.x) > max_half_size.x)
		{
			max_half_size.x = vertex.position.x;
		}
		if (abs(vertex.position.y) > max_half_size.y)
		{
			max_half_size.y = vertex.position.y;
		}
		if (abs(vertex.position.z) > max_half_size.z)
		{
			max_half_size.z = vertex.position.z;
		}
	}

	//aabb_volume = std::make_shared<AABBVolume>(vec3(max_half_size.length()));
	aabb_volume = vec3(glm::length(max_half_size));
	//AABBVolume* newone = &*aabb_volume;
}
