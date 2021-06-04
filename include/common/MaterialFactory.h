#pragma once
#include "Material.h"

class MaterialFactory
{
public:
	virtual Material* CreateMaterial() = 0;

	virtual ~MaterialFactory() {}	//- 生命成private或protected无法在栈中生产

protected:

};

