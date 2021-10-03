#pragma once
#include "Texture.h"
struct PlaneTexture : public Texture
{
public:
	PlaneTexture(TextureType type_ = TextureType::EMPTY2D, TextureFile* file_ = nullptr);

	virtual bool Buffer() override;
};

