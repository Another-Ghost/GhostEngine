#pragma once
#include "GeometryMesh.h"
class SphereGeometryMesh : public GeometryMesh
{
public:
	SphereGeometryMesh(int x_segment_num_ = 64, int y_segment_num_ = 64);

	void InitializeData() override;

	unsigned int x_segment_num;
	unsigned int y_segment_num;

	void InitializeAABB() override;
};

