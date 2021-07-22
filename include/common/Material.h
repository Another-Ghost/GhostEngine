#pragma once
#include "Prerequisite.h"
#include "PlaneTexture.h"
#include <vector>

enum class MaterialType
{
	BASIC,
	PBR,
};

struct Material
{
	Material(MaterialType type_ = MaterialType::BASIC) : type(type_){}
	//std::vector<Texture*> textures;

	//Material(vector<Texture*> texs = vector<Texture*>()) :textures(texs) { }
	MaterialType type;
	
	virtual ~Material();
};

