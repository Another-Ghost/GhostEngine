#include "RenderModule.h"
#include "RenderManager.h"
#include "RenderUnit.h"

void RenderModule::Update(float dt)
{
	for (const auto& render_unit : render_unit_array)
	{
		render_unit->Update(dt);
	}

	for (const auto& child : child_array)
	{
		child->Update(dt);
	}
}

void RenderModule::AddChild(RenderModule* child)
{
	child_array.emplace_back(child); 
	child->SetParent(this);
}

void RenderModule::AddRenderUnit(RenderUnit* ru)
{
	render_unit_array.emplace_back(ru);
	ru->SetParent(this);
}

Transform RenderModule::GetWorldTransform()
{
	return parent->GetWorldTransform() * local_transform;
}


