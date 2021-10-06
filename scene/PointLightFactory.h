#pragma once
#include "LightFactory.h"
class PointLightFactory : public LightFactory
{
public:
	Light* CreateLight() const override;
};

