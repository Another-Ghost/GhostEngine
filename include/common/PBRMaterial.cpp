#include "PBRMaterial.h"

void PBRMaterial::Buffer()
{
	specular_map->Buffer();
	normal_map->Buffer();
	metalness_map->Buffer();
	roughness_map->Buffer();
	ao_map->Buffer();
}
