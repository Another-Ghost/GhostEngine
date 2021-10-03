#pragma once
#include "MeshFactory.h"
#include "CubeGeometryMesh.h"

class CubeGeometryMeshFactory : public MeshFactory
{
public:
    virtual Mesh* CreateMesh() const override;
};

