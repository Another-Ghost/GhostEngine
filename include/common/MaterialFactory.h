#pragma once
#include "Material.h"

class MaterialFactory
{
public:
	virtual Material* CreateMaterial() = 0;

protected:
	virtual ~MaterialFactory() {}
};

