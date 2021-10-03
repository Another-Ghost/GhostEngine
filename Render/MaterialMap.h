#pragma once
#include "PlaneTexture.h"

struct MaterialMap : PlaneTexture
{
	MaterialMap(TextureType type_, TextureFile* file_ = nullptr);
	
	//bool Buffer() override;
};

