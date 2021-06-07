#pragma once
#include "Prerequisite.h"
#include "Unit.h"

class RenderUnit : public Unit
{
public:
	RootRenderModule* root_render_module;	//? 应不应该改成列表

	void AttachRenderModule(RootRenderModule* root_render_module_);

	virtual void Update(float dt) override;
};

