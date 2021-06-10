#include "RenderUnit.h"
#include "RootRenderModule.h"
#include "SceneManager.h"

RenderUnit::RenderUnit()
{
	SceneManager::GetSingleton().AddRenderUnit(this);
}

RenderUnit::~RenderUnit()
{
	parent->RemoveChild(this);
}

void RenderUnit::AttachRenderModule(RootRenderModule* root_render_module_)
{
	root_render_module = root_render_module_;
	root_render_module->SetParentUnit(this);
}

void RenderUnit::Update(float dt)
{
	root_render_module->Update(dt);
}
