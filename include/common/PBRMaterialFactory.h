#pragma once
#include "MaterialFactory.h"

class PBRMaterialFactory : public MaterialFactory
{
public:
	Material* CreateMaterial() const override;

};

