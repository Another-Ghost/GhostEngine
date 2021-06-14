#pragma once
#include "Prerequisite.h"
#include "Texture.h"


struct CubeMap : Texture
{
	CubeMap(TextureType type_ = TextureType::CUBEMAP, TextureFile* path_ = nullptr);
	
	virtual bool Buffer() override;
};

