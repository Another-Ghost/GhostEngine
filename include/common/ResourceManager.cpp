#include "ResourceManager.h"
#include "Texture.h"
#include "Material.h"
#include "PBRMaterialFactory.h"
#include <stb_image.h>

template<> ResourceManager* Singleton<ResourceManager>::singleton = nullptr;
Texture* ResourceManager::CreateTexture(const string& path, TextureType type)
{
    Texture* texture = new Texture(path, type);
    texture_array.emplace_back(texture);

    return nullptr;
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
