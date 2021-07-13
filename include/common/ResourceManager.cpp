#include "ResourceManager.h"
#include "EquirectangularMap.h"
#include "Material.h"
#include "PBRMaterialFactory.h"
#include "MeshFactory.h"
#include "EquirectangularMap.h"
#include "MaterialMap.h"
#include "CubeMap.h"
#include "TextureFile.h"
#include "RootRenderModule.h"
#include "TriangleMesh.h"
#include "RenderUnit.h"
#include "BasicMaterialFactory.h"

template<> ResourceManager* Singleton<ResourceManager>::singleton = nullptr;
ResourceManager::ResourceManager()
{
    stbi_set_flip_vertically_on_load(true);

    gltf_loader = std::make_unique<GLTFLoader>();
}
Texture* ResourceManager::CreateTexture(TextureType type, TextureFile* file, bool b_buffer)
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

    Texture* texture = nullptr;
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
    case TextureType::BASECOLOR:
    case TextureType::AO:
    case TextureType::METALNESS:
    case TextureType::NORMAL:
    case TextureType::ROUGHNESS:
    case TextureType::METALNESSROUGHNESS:
	case TextureType::DIFFUSE:
	case TextureType::SPECULAR:
        texture = new MaterialMap(type, file);
        break;
    default:
        break;
    }

    if (b_buffer)
    {
        texture->Buffer();
    }

    texture_array.emplace_back(texture);

    return texture;
}

Texture* ResourceManager::CreateMetalnessRoughnessMap(Texture* metalness_map, Texture* roughness_map)
{

    return nullptr;
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
    case MaterialType::BASIC:
        factory = std::make_unique<BasicMaterialFactory>();
        break;

    case MaterialType::PBR:
        factory = std::make_unique<PBRMaterialFactory>();
    	break;
    }

	Material* material = factory->CreateMaterial();
	material_array.emplace_back(material);
	return material;

}

Material* ResourceManager::CreateMaterial(const MaterialFactory& mat_factory)
{
    Material* material = mat_factory.CreateMaterial();
    material_array.emplace_back(material);
    return material;
}

Mesh* ResourceManager::CreateMesh(const MeshFactory& mesh_factory)
{

    Mesh* mesh = mesh_factory.CreateMesh();
    mesh_array.emplace_back(mesh);
    return mesh;
}

TriangleMesh* ResourceManager::CreateTriangleMesh(bool b_buffer, const vector<ExpandedVertex>& vertex_array, const vector<unsigned int>& index_array)
{
    TriangleMesh* mesh = new TriangleMesh(vertex_array, index_array);
    if (b_buffer)
    {
        mesh->Buffer();
    }
    mesh_array.emplace_back(mesh);
    return mesh;
}



RenderModule* ResourceManager::CreateRenderModule(RenderModule* parent, RenderModuleType type /*= RenderModuleType::DEFAULT*/)
{
    RenderModule* render_module = nullptr;
    switch (type)
    {
    case RenderModuleType::DEFAULT:
        render_module = new RenderModule();
        break;
    case RenderModuleType::ROOT:
        render_module = new RootRenderModule();
        break;
    default:
        break;
    }

    if (parent)
    {
        parent->AddChild(render_module);
    }
    //render_module->SetParent(parent);
    render_module_array.emplace_back(render_module);

    return render_module;
}

RenderUnit* ResourceManager::CreateRenderUnit(RenderModule* parent, Mesh* mesh, Material* material)
{
	RenderUnit* ru = new RenderUnit(parent, mesh, material);
    parent->AddRenderUnit(ru);
	render_unit_array.push_back(ru);
	return ru;
}



//CubeMap* ResourceManager::CreateCubeMap(int width, int height)
//{
//    CubeMap* cube_map = new CubeMap();
//    //cube_map->Buffer();
//    cube_map_array.emplace_back(cube_map);
//    return cube_map;
//}
