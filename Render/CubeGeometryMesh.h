#pragma once
#include "GeometryMesh.h"

class CubeGeometryMesh : public GeometryMesh
{
public:

	CubeGeometryMesh();

	void InitializeData() override;

	void Draw(Shader* shader) override;

	void InitializeAABB() override;
};

