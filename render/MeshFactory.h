#pragma once
#include "Mesh.h"

class MeshFactory
{
public:
	virtual Mesh* CreateMesh() const = 0;
};

