#pragma once
#include "Prerequisite.h"
#include "Singleton.h"
#include "Material.h"
#include "Texture.h"
#include "CubeMap.h"

struct TexturePredicate
{
	bool operator()(Texture* tex_a, Texture* tex_b)
	{
		if (tex_a && tex_b)
		{
			return tex_a > tex_b;
		}
		return false;
	}

};

class ResourceManager : public Singleton<ResourceManager>
{
	vector<Mesh*> mesh_set;

	vector<Shader*> shader_array;

	vector<Texture*> texture_array;

	vector<Material*> material_array;

	vector<CubeMap*> cube_map_array;

public:
	ResourceManager();

	Texture* CreateTexture(const string& path, TextureType type);



	Material* CreateMaterial(MaterialType type);

	Mesh* CreateMesh(const MeshFactory& mesh_factory);

	CubeMap* CreateCubeMap(int width, int height);
};

