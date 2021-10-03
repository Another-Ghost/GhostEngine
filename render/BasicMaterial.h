#pragma once
#include "Material.h"
struct BasicMaterial : public Material
{
	vector<PlaneTexture*> diffuse_map_array;
	vector<PlaneTexture*> specular_map_array;
	vector<PlaneTexture*> normal_map_array;

};

