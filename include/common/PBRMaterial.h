#pragma once
#include "Material.h"

struct PBRMaterial : public Material
{
	Texture* specular_map;
	Texture* roughness_map;
	Texture* normal_map;
	Texture* metalness_map;
	Texture* ao_map;
};

