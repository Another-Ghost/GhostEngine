#include "TransformableModule.h"

Transform TransformableModule::GetWorldTransform()
{
	if (parent)
	{
		return parent->GetWorldTransform() * local_transform;
	}
	else
	{
		return local_transform;
	}
}
