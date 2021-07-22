#pragma once
#include "Material.h"

struct PBRMaterial : Material
{
	PBRMaterial(): Material(MaterialType::PBR){}

	PlaneTexture* basecolor_map;
	PlaneTexture* roughness_map;
	PlaneTexture* normal_map;
	PlaneTexture* metalness_map;
	PlaneTexture* ao_map{ nullptr };
	PlaneTexture* metalness_roughness_map{ nullptr };
	PlaneTexture* emissive_map{ nullptr };

	bool b_metallic_rough{ false };

	void Buffer();

	//static Texture* CreateMetalnessRoughnessMap(Texture* metalness_tex, Texture* roughness_tex);

	PlaneTexture* CreateMetalnessRoughnessMap();

};

