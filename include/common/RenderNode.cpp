#include "RenderNode.h"
#include "RootRenderModule.h"
#include "SceneManager.h"

RenderNode::RenderNode()
{
	SceneManager::GetSingleton().AddRenderNode(this);
}

RenderNode::~RenderNode()
{
	parent->RemoveChild(this);
}

void RenderNode::AttachRenderModule(RootRenderModule* root_render_module_)
{
	root_render_module = root_render_module_;
	root_render_module->SetParentNode(this);
}

void RenderNode::Update(float dt)
{
	if (root_render_module)
	{
		root_render_module->Update(dt);
	}
}
