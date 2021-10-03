#pragma once
#include "Camera.h"

class CameraFactory
{
public:
	virtual Camera* CreateCamera() const;

	virtual ~CameraFactory() {}

protected:


};

