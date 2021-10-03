#pragma once
#include "Texture.h"


struct CubeMap : Texture
{
	CubeMap(int width_, int height_, TextureType type_ = TextureType::CUBEMAP, TextureFile* file_ = nullptr);
	
	virtual bool Buffer() override;
};

