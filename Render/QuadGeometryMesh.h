#pragma once
#include "GeometryMesh.h"
class QuadGeometryMesh : public GeometryMesh
{
public:
	QuadGeometryMesh();

	void Initialize() override;

	void Draw(Shader* shader) override;

private:
	void InitializeAABB() override;
};

