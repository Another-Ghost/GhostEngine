#pragma once
#include "MaterialFactory.h"
#include "BasicMaterial.h"
class BasicMaterialFactory : public MaterialFactory
{
	Material* CreateMaterial() const override { return new BasicMaterial(); }
};

