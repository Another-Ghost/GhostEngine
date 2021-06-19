#include "ResourceManager.h"
#include "EquirectangularMap.h"
#include "Material.h"
#include "PBRMaterialFactory.h"
#include "MeshFactory.h"
#include "EquirectangularMap.h"
#include "MaterialMap.h"
#include "CubeMap.h"
#include "TextureFile.h"

template<> ResourceManager* Singleton<ResourceManager>::singleton = nullptr;
ResourceManager::ResourceManager()
{
    stbi_set_flip_vertically_on_load(true);
}
Texture* ResourceManager::CreateTexture(TextureType type, TextureFile* file)
{
    if (file != nullptr)
    {
        for (const auto& tex : texture_array)
        {
            if (file == tex->file)
            {
                return tex;
            }
        }
    }

    Texture* texture;
    switch (type)
    {
    case TextureType::EMPTY2D:
        texture = new Texture(type, file);
        break;
    case TextureType::EQUIRECTANGULARMAP:
        texture = new EquirectangularMap(type, file);
        break;
    case TextureType::CUBEMAP:
        texture = new CubeMap(type, file);
        break;
    case TextureType::ALBEDO :
    case TextureType::AO:
    case TextureType::METALNESS:
    case TextureType::NORMAL:
    case TextureType::ROUGHNESS:
        texture = new MaterialMap(type, file);
        break;
    default:
        break;
    }

    texture_array.emplace_back(texture);

    return texture;
}
TextureFile* ResourceManager::CreateTextureFile(const string& path, TextureFileType type)
{
    for (const auto& file : texture_file_array)
    {
        if (path == file->path)
        {
            return file;
        }
    }

    TextureFile* texture_file;
    switch (type)
    {
    case TextureFileType::LDR:
        texture_file = new LDRTextureFile(path, type);
    	break;
    case TextureFileType::HDR:
        texture_file = new HDRTextureFile(path, type);
        break;
    default:
        break;
    }
    texture_file_array.push_back(texture_file);

    return texture_file;
}



Material* ResourceManager::CreateMaterial(MaterialType type)
{
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
    mesh_array.emplace_back(mesh);
    return mesh;
}

//CubeMap* ResourceManager::CreateCubeMap(int width, int height)
//{
//    CubeMap* cube_map = new CubeMap();
//    //cube_map->Buffer();
//    cube_map_array.emplace_back(cube_map);
//    return cube_map;
//}
