#pragma once
#include "Prerequisite.h"
#include "Node.h"

class RenderNode : public Node
{
public:
	RootRenderModule* root_render_module;	//? 应不应该改成列表

	RenderNode();

	~RenderNode();

	void AttachRenderModule(RootRenderModule* root_render_module_);

	virtual void Update(float dt) override;
};

