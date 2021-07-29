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

	AABBModule(AABBVolume volume_ = AABBVolume()) :volume(volume_) {}

	vec3 GetWorldPosition();

};

