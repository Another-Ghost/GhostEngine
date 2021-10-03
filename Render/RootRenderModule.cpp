#include "RootRenderModule.h"
#include "Node.h"
Transform RootRenderModule::GetWorldTransform()
{
	if (parent)
	{
//#ifdef DEBUG_MODE
//		Transform matrix = parent->GetWorldTransform() * local_transform;
//		matrix.PrintMatrix();
//#endif // DEBUG_MODE

		return parent->GetWorldTransform() * local_transform;
	}
	else
	{
		return local_transform;
	}
}
