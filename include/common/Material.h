#pragma once
#include "Prerequisite.h"
#include "Texture.h"
#include <vector>

enum class MaterialType
{
	PBR,
};

struct Material
{

	//std::vector<Texture*> textures;

	//Material(vector<Texture*> texs = vector<Texture*>()) :textures(texs) { }
	
	virtual ~Material();
};

