#pragma once
#include "Module.h"

class TransformableModule : public Module
{

public:
	Transform local_transform;

	virtual Transform GetWorldTransform(); 

	void SetParent(TransformableModule* parent_) { parent = parent; }

protected:

	TransformableModule* parent{ nullptr };
};

