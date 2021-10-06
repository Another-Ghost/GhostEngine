#include "RenderNode.h"
#include "SceneManager.h"
#include "RenderModule.h"

RenderNode::RenderNode()
{
	SceneManager::GetSingleton().AddRenderNode(this);
}

RenderNode::~RenderNode()
{
	parent->RemoveChild(this);
}

void RenderNode::AttachRenderModule(RenderModule* root_render_module_)
{
	root_render_module = root_render_module_;
	root_render_module->SetParent(this);
}

void RenderNode::Update(float dt)
{
	if (root_render_module)
	{
		root_render_module->Update(dt);
	}

	Node::Update(dt);
}
