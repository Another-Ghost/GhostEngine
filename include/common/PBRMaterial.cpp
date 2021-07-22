#include "PBRMaterial.h"
#include "ResourceManager.h"
#include "RenderManager.h"

void PBRMaterial::Buffer()
{
	basecolor_map->Buffer();
	normal_map->Buffer();
	metalness_map->Buffer();
	roughness_map->Buffer();
	ao_map->Buffer();
}

PlaneTexture* PBRMaterial::CreateMetalnessRoughnessMap()
{
	metalness_roughness_map= ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::METALNESSROUGHNESS);
	//roughness_map->data_format = GL_RG;
	metalness_roughness_map->width = roughness_map->width;
	metalness_roughness_map->height = roughness_map->height;
	metalness_roughness_map->b_genarate_mipmap = false;
	metalness_roughness_map->Buffer();
	RenderManager::GetSingleton().CombineChannels(metalness_roughness_map, roughness_map, metalness_map);
	return metalness_roughness_map;
}

//Texture* PBRMaterial::CreateMetalnessRoughnessMap(Texture* metalness_tex, Texture* roughness_tex)
//{
//	Texture* out_tex = ResourceManager::GetSingleton().CreateTexture(TextureType::EMPTY2D);
//
//	return nullptr;
//}
