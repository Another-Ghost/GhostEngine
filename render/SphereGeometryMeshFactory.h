#pragma once
#include "MeshFactory.h"
class SphereGeometryMeshFactory : public MeshFactory
{
public:
	SphereGeometryMeshFactory();
	Mesh* CreateMesh() const override;

};

