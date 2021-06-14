#pragma once
#include "Texture.h"
struct EquirectangularMap : public Texture
{
    EquirectangularMap(TextureType type_ = TextureType::EQUIRECTANGULARMAP, TextureFile* file_ = nullptr);
    bool Buffer() override;
};

