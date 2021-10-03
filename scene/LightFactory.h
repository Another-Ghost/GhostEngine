#pragma once
#include "Light.h"

class LightFactory
{
public:
	virtual Light* CreateLight() const;
};

