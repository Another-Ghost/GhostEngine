#pragma once
#include "Texture.h"

struct MaterialMap : Texture
{
	MaterialMap(TextureType type_, TextureFile* file_ = nullptr, bool b_buffer = true);
	
	//bool Buffer() override;
};

