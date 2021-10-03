#include "AABBModule.h"
#include "RenderModule.h"

Transform AABBModule::GetWorldTransform()
{
	return Transform();
}

AABBModule::AABBModule(AABBVolume volume_ /*= AABBVolume()*/, const vec3& local_pos /*= { 0.f, 0.f, 0.f }*/)
	:volume(volume_)
{
	local_transform.SetPosition(local_pos);
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
