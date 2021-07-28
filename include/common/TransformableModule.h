#pragma once
#include "Module.h"

class TransformableModule : public Module
{

public:
	Transform local_transform;

	virtual Transform GetWorldTransform(); 

protected:

	TransformableModule* parent;
};

