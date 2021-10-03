#pragma once
#include "PlaneTexture.h"
struct EquirectangularMap : public PlaneTexture
{
    EquirectangularMap(TextureType type_ = TextureType::EQUIRECTANGULARMAP, TextureFile* file_ = nullptr);
    bool Buffer() override;
};

