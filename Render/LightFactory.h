#pragma once
#include "Prerequisite.h"

class LightFactory
{
public:
	virtual Light* CreateLight() const;
};

