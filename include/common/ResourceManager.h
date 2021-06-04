#pragma once
#include "Prerequisite.h"
#include "Singleton.h"
#include "Material.h"

class ResourceManager : public Singleton<ResourceManager>
{
	vector<Mesh*> mesh_array;

	vector<Shader*> shader_array;

	vector<Texture*> texture_array;

	vector<Material*> material_array;

public:
	Texture* CreateTexture(const string& path, TextureType type);

	Material* CreateMaterial(MaterialType type);


};

