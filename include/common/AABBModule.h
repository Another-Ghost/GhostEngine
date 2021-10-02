#pragma once
#include "Prerequisite.h"
#include "AABBVolume.h"
#include "TransformableModule.h"
class AABBModule : public TransformableModule
{
private:
	Transform GetWorldTransform() override;

public:
	//Transform GetWorldTransform() = delete;	//Œ•∑¥is-a‘≠‘Ú

	AABBVolume volume;

	AABBModule(AABBVolume volume_ = AABBVolume(), const vec3& local_pos = { 0.f, 0.f, 0.f });
	
	vec3 GetWorldPosition();

};

