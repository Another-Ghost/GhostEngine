#include "RenderModule.h"
#include "RenderManager.h"
#include "RenderUnit.h"

void RenderModule::Update(float dt)
{
	for (const auto& render_unit : render_unit_array)
	{
		render_unit->Update(dt);
	}
}
