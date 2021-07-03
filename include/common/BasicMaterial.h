#pragma once
#include "Material.h"
struct BasicMaterial : public Material
{

	vector<Texture*> diffuse_map_array;
	vector<Texture*> specular_map_array;
	vector<Texture*> normal_map_array;

};

