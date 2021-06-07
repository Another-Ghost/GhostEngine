#include "RenderUnit.h"
#include "RootRenderModule.h"

void RenderUnit::AttachRenderModule(RootRenderModule* root_render_module_)
{
	root_render_module = root_render_module_;
	root_render_module->SetParentUnit(this);
}

void RenderUnit::Update(float dt)
{
	root_render_module->Update(dt);
}
