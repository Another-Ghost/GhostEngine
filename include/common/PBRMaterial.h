#pragma once
#include "Material.h"

struct PBRMaterial : Material
{
	PBRMaterial(): Material(MaterialType::PBR){}

	Texture* albedo_map;
	Texture* roughness_map;
	Texture* normal_map;
	Texture* metalness_map;
	Texture* ao_map;


	void Buffer();
};

