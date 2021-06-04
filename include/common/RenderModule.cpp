#include "RenderModule.h"
#include "RenderManager.h"

void RenderModule::Update(float dt)
{
	if (b_rendered == true)
	{
		RenderManager::GetSingleton().InsertRenderModule(this);
	}
}
