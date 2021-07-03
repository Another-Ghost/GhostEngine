#include "RenderUnit.h"
#include "RenderManager.h"

void RenderUnit::Update(float dt)
{
	if (b_rendered == true)
	{
		RenderManager::GetSingleton().InsertRenderUnit(this);
	}
}
