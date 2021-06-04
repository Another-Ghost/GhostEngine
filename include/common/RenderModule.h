#pragma once
#include "Module.h"

class RenderModule : public Module
{
public:
	RenderModule() : b_rendered(true) {}

	Mesh* mesh;

	Material* material;

	Transform transform;

	virtual void Update(float dt) override;

	bool b_rendered;
};

