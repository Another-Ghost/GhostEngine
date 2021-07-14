#pragma once
#include "Material.h"

struct PBRMaterial : Material
{
	PBRMaterial(): Material(MaterialType::PBR){}

	Texture* basecolor_map;
	Texture* roughness_map;
	Texture* normal_map;
	Texture* metalness_map;
	Texture* ao_map{ nullptr };
	Texture* metalness_roughness_map{ nullptr };
	Texture* emissive_map{ nullptr };

	bool b_metallic_rough{ false };

	void Buffer();

	//static Texture* CreateMetalnessRoughnessMap(Texture* metalness_tex, Texture* roughness_tex);

	Texture* CreateMetalnessRoughnessMap();

};

