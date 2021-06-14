#pragma once
#include "Texture.h"

struct MaterialMap : Texture
{
	MaterialMap(TextureType type_, TextureFile* file_ = nullptr);
	
	bool Buffer() override;
};

