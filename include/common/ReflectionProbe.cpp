#include "ReflectionProbe.h"
#include "ResourceManager.h"

ReflectionProbe::ReflectionProbe(const vec3& position, const AABBModule& aabb_):
	LightProbe(position, aabb_)
{

}

void ReflectionProbe::Initialize()
{
	cubemap = ResourceManager::GetSingleton().CreateCubeMap(128, 128);


}

void ReflectionProbe::Capture()
{
	
}
