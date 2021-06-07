#pragma once
#include "MeshFactory.h"
class SphereGeometryMeshFactory : public MeshFactory
{
public:
	SphereGeometryMeshFactory(float radius_ = 1.f);
	Mesh* CreateMesh() const override;

	float radius;
};

