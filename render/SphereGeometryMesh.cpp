#include "SphereGeometryMesh.h"

SphereGeometryMesh::SphereGeometryMesh(int x_segment_num_, int y_segment_num_):
	x_segment_num(x_segment_num_), y_segment_num(y_segment_num_)
{
	//Initialize();

	//if (!b_buffered)
	//{
	//	Buffer();
	//}
}

void SphereGeometryMesh::InitializeData()
{
	constexpr float PI = glm::pi<float>();
	for (int y = 0; y <= y_segment_num; ++y)
	{
		for (int x = 0; x <= x_segment_num; ++x)
		{
			float x_segment = (float)x / (float)x_segment_num;
			float y_segment = (float)y / (float)y_segment_num;
			float x_pos = std::cos(x_segment * 2.f * PI) * std::sin(y_segment * PI);
			float y_pos = std::cos(y_segment * PI);
			float z_pos = std::sin(x_segment * 2.f * PI) * std::sin(y_segment * PI);
			
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

void SphereGeometryMesh::InitializeAABB()
{
	//aabb_volume = std::make_shared<AABBVolume>(vec3{1.f});
	aabb_volume = vec3{ 1.f };
}


