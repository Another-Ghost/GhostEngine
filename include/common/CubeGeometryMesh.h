#pragma once
#include "GeometryMesh.h"

class CubeGeometryMesh : public GeometryMesh
{
public:

	CubeGeometryMesh();

	void Initialize() override;

	void Draw(Shader* shader) override;
};

