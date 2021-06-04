#pragma once
#include "Module.h"

class RenderModule : public Module
{
public:
	
	Mesh* mesh;

	Material* material;

	Transform offset_transform;


	virtual void Update(float dt) override;


};

