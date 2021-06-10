#include "ResourceManager.h"
#include "HDRTexture.h"
#include "Material.h"
#include "PBRMaterialFactory.h"
#include "MeshFactory.h"

template<> ResourceManager* Singleton<ResourceManager>::singleton = nullptr;
ResourceManager::ResourceManager()
{
    stbi_set_flip_vertically_on_load(true);
}
Texture* ResourceManager::CreateTexture(const string& path, TextureType type)
{
    for (const auto& tex : texture_array)
    {
        if (path == tex->path)
        {
            return tex;
        }
    }

    Texture* texture;
    if (type == TextureType::HDRMAP)
    {
        texture = new HDRTexture(path, type);
    }
    else
    {
        texture = new Texture(path, type);
    }
    texture->LoadData();
	texture->Buffer();
    
    texture_array.emplace_back(texture);

    return texture;
}

Material* ResourceManager::CreateMaterial(MaterialType type)
{
    Material* material;
    unique_ptr<MaterialFactory> factory;
    switch (type)
    {
    case MaterialType::PBR:
        factory = std::make_unique<PBRMaterialFactory>();
    	break;
    }

    return factory->CreateMaterial();
}

Mesh* ResourceManager::CreateMesh(const MeshFactory& mesh_factory)
{
    Mesh* mesh = mesh_factory.CreateMesh();
    mesh_set.emplace_back(mesh);
    return mesh;
}

CubeMap* ResourceManager::CreateCubeMap(int width, int height)
{
    CubeMap* cube_map = new CubeMap(width, height);
    cube_map->Buffer();
    cube_map_array.emplace_back(cube_map);
    return cube_map;
}
