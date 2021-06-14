#include "CubeGeometryMesh.h"

CubeGeometryMesh::CubeGeometryMesh()
{
	Initialize();

	if (!b_buffered)
	{
		Buffer();
	}
}

void CubeGeometryMesh::Initialize()
{
	//vector<vec3> postion_array = {
	//	{-1.f, -1.f, -1.f},
	//	{1.f, -1.f, -1.f},
	//	{-1.f,  1.f, -1.f},
	//	{1.f,  1.f, -1.f},
	//	{-1.f, -1.f,  1.f},
	//	{1.f, -1.f,  1.f},
	//	{-1.f,  1.f,  1.f},
	//	{1.f,  1.f,  1.f}
	//};

	vertex_array = {
		// back face
		{{-1.f, -1.f, -1.f},	{0.0f,  0.0f}, {-1.0f, 0.0f, 0.0f} }, // bottom-left
		{{1.f,  1.f, -1.f},	{0.0f,  0.0f}, {-1.0f, 1.0f, 1.0f} }, // top-right
		{{1.f, -1.f, -1.f},  {0.0f,  0.0f}, {-1.0f, 1.0f, 0.0f}}, // bottom-right         
		{{1.f,  1.f, -1.f},  {0.0f,  0.0f}, {-1.0f, 1.0f, 1.0f}}, // top-right
		{{-1.f, -1.f, -1.f},  {0.0f,  0.0f}, {-1.0f, 0.0f, 0.0f}}, // bottom-left
		{{-1.f,  1.f, -1.f},  {0.0f,  0.0f}, {-1.0f, 0.0f, 1.0f}}, // top-left
		// front face
		{{-1.f, -1.f,  1.f},  {0.0f,  0.0f},  {1.0f, 0.0f, 0.0f}}, // bottom-left
		{{1.f, -1.f,  1.f},  {0.0f,  0.0f},  {1.0f, 1.0f, 0.0f}}, // bottom-right
		{{1.f,  1.f,  1.f},  {0.0f,  0.0f},  {1.0f, 1.0f, 1.0f}}, // top-right
		{{1.f,  1.f,  1.f},  {0.0f,  0.0f},  {1.0f, 1.0f, 1.0f}}, // top-right
		{{-1.f,  1.f,  1.f},  {0.0f,  0.0f},  {1.0f, 0.0f, 1.0f}}, // top-left
		{{-1.f, -1.f,  1.f},  {0.0f,  0.0f},  {1.0f, 0.0f, 0.0f}}, // bottom-left
		// left face
		{ {-1.f,  1.f,  1.f}, {-1.0f,  0.0f},  {0.0f, 1.0f, 0.0f }}, // top-right
		{ {-1.f,  1.f, -1.f}, {-1.0f,  0.0f},  {0.0f, 1.0f, 1.0f }}, // top-left
		{ {-1.f, -1.f, -1.f}, {-1.0f,  0.0f},  {0.0f, 0.0f, 1.0f }}, // bottom-left
		{ {-1.f, -1.f, -1.f}, {-1.0f,  0.0f},  {0.0f, 0.0f, 1.0f }}, // bottom-left
		{ {-1.f, -1.f,  1.f}, {-1.0f,  0.0f},  {0.0f, 0.0f, 0.0f }}, // bottom-right
		{ {-1.f,  1.f,  1.f}, {-1.0f,  0.0f},  {0.0f, 1.0f, 0.0f }}, // top-right
		// right face
		{ {1.f,  1.f,  1.f},  {1.0f,  0.0f},  {0.0f, 1.0f, 0.0f }}, // top-left
		{ {1.f, -1.f, -1.f},  {1.0f,  0.0f},  {0.0f, 0.0f, 1.0f }}, // bottom-right
		{ {1.f,  1.f, -1.f},  {1.0f,  0.0f},  {0.0f, 1.0f, 1.0f }}, // top-right         
		{ {1.f, -1.f, -1.f},  {1.0f,  0.0f},  {0.0f, 0.0f, 1.0f }}, // bottom-right
		{ {1.f,  1.f,  1.f},  {1.0f,  0.0f},  {0.0f, 1.0f, 0.0f }}, // top-left
		{ {1.f, -1.f,  1.f},  {1.0f,  0.0f},  {0.0f, 0.0f, 0.0f }}, // bottom-left     
		// bottom face
		{ {-1.f, -1.f, -1.f},  {0.0f, -1.0f},  {0.0f, 0.0f, 1.0f }}, // top-right
		{ {1.f, -1.f, -1.f},  {0.0f, -1.0f},  {0.0f, 1.0f, 1.0f }}, // top-left
		{ {1.f, -1.f,  1.f},  {0.0f, -1.0f},  {0.0f, 1.0f, 0.0f }}, // bottom-left
		{ {1.f, -1.f,  1.f},  {0.0f, -1.0f},  {0.0f, 1.0f, 0.0f }}, // bottom-left
		{ {-1.f, -1.f,  1.f},  {0.0f, -1.0f},  {0.0f, 0.0f, 0.0f }}, // bottom-right
		{ {-1.f, -1.f, -1.f},  {0.0f, -1.0f},  {0.0f, 0.0f, 1.0f }}, // top-right
		// top face
		{ {-1.f,  1.f, -1.f},  {0.0f,  1.0f},  {0.0f, 0.0f, 1.0f }}, // top-left
		{ {1.f,  1.f , 1.f},  {0.0f,  1.0f},  {0.0f, 1.0f, 0.0f }}, // bottom-right
		{ {1.f,  1.f, -1.f},  {0.0f,  1.0f},  {0.0f, 1.0f, 1.0f }}, // top-right     
		{ {1.f,  1.f,  1.f},  {0.0f,  1.0f},  {0.0f, 1.0f, 0.0f }}, // bottom-right
		{ {-1.f,  1.f, -1.f},  {0.0f,  1.0f},  {0.0f, 0.0f, 1.0f }}, // top-left
		{ {-1.f,  1.f,  1.f},  {0.0f,  1.0f},  {0.0f, 0.0f, 0.0f }}  // bottom-left        
	};

	index_array = {
		0, 1, 2, 3, 4, 5, 
		6, 7, 8, 9, 10, 11, 
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35
	}; //? ´íÎó
}

void CubeGeometryMesh::Draw()
{
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

