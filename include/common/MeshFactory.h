#pragma once
#include "Prerequisite.h"

class MeshFactory
{
public:
	virtual Mesh* CreateMesh() const;
};

