#include "RenderUnit.h"
#include "RenderModule.h"

void RenderUnit::AttachRenderModule(RenderModule* render_module_)
{
	render_module = render_module_;
	render_module->SetParent(this);
}

void RenderUnit::Update(float dt)
{
	render_module->Update(dt);
}
