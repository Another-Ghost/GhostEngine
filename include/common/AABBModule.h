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

	vec3 GetWorldPosition();

};

