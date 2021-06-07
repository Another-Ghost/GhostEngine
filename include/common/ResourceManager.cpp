#include "ResourceManager.h"
#include "Texture.h"
#include "Material.h"
#include "PBRMaterialFactory.h"
#include "MeshFactory.h"
#include <stb_image.h>

template<> ResourceManager* Singleton<ResourceManager>::singleton = nullptr;
Texture* ResourceManager::CreateTexture(const string& path, TextureType type)
{
    for (const auto& tex : texture_set)
    {
        if (path == tex->path)
        {
            return tex;
        }
    }

    Texture* texture = new Texture(path, type);
    texture_set.emplace_back(texture);

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
