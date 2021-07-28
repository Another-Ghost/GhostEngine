#include "AABBModule.h"
#include "RenderModule.h"

Transform AABBModule::GetWorldTransform()
{
	return Transform();
}

vec3 AABBModule::GetWorldPosition()
{
	if (parent)
	{
		return (parent->GetWorldTransform() * local_transform).GetPosition();
	}
	else
	{
		return local_transform.GetPosition();
	}
}
