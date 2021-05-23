#pragma once
#include "Texture.h"
#include <vector>

class Material
{
public:
	std::vector<Texture> textures;

	Material(std::vector<Texture> texs) :textures(texs) { }
};

