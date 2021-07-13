#include "RootRenderModule.h"
#include "Node.h"
Transform RootRenderModule::GetWorldTransform()
{
	if (parent)
	{
		return local_transform * parent->GetWorldTransform();
	}
	else
	{
		return local_transform;
	}
}
