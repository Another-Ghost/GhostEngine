#include "SphereGeometryMesh.h"

SphereGeometryMesh::SphereGeometryMesh(float radius_, int x_segment_num_, int y_segment_num_):
	radius(radius_), x_segment_num(x_segment_num_), y_segment_num(y_segment_num_)
{
	Initialize();
}

void SphereGeometryMesh::Initialize()
{
	const float PI = glm::pi<float>();
	for (int y = 0; y < y_segment_num; ++y)
	{
		for (int x = 0; x < x_segment_num; ++x)
		{
			float x_segment = (float)x / (float)x_segment_num;
			float y_segment = (float)y / (float)y_segment_num;
			float x_pos = radius * cos(x_segment * 2.f * PI) * sin(y_segment * PI);
			float y_pos = radius * cos(y_segment * PI);
			float z_pos = radius * sin(x_segment * 2.f * PI) * sin(y_segment * PI);
			
			Vertex v{ vec3(x_pos, y_pos, z_pos), vec2(x_segment, y_segment), vec3(x_pos, y_pos, z_pos) };
			vertex_array.push_back(v);
		}
	}

	bool odd_row = false;
	for (int y = 0; y < y_segment_num; ++y)
	{
		if (!odd_row)
		{
			for (int x = 0; x <= x_segment_num; ++x)
			{
				index_array.push_back(y * (x_segment_num + 1) + x);
				index_array.push_back((y + 1) * (x_segment_num + 1) + x);
			}
		}
		else
		{
			for (int x = x_segment_num; x >= 0; --x)
			{
				index_array.push_back((y + 1) * (x_segment_num + 1) + x);
				index_array.push_back(y * (x_segment_num + 1) + x);
			}
		}
		odd_row = !odd_row;
	}
}

void SphereGeometryMesh::Buffer()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_array.size() * sizeof(Vertex), &vertex_array[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_array.size() * sizeof(unsigned int), &index_array[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //启用编号0的顶点属性，0对应于shader中的 layout(location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glBindVertexArray(0);
}
