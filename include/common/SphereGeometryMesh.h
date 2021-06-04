#pragma once
#include "GeometryMesh.h"
class SphereGeometryMesh : public GeometryMesh
{
public:
	SphereGeometryMesh(float radius_ = 1.f, int x_segment_num_ = 64, int y_segment_num_ = 64);

	void Initialize();

	void Buffer();

	unsigned int radius;

	unsigned int x_segment_num;
	unsigned int y_segment_num;


private:

	unsigned int vbo_id;
	unsigned int ebo_id;


};

