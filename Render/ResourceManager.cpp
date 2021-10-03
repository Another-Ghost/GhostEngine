#include "ResourceManager.h"
#include "EquirectangularMap.h"
#include "Material.h"
#include "PBRMaterialFactory.h"
#include "MeshFactory.h"
#include "EquirectangularMap.h"
#include "MaterialMap.h"
#include "CubeMap.h"
#include "AttachmentTexture.h"
#include "TextureFile.h"
#include "RootRenderModule.h"
#include "TriangleMesh.h"
#include "RenderUnit.h"
#include "BasicMaterialFactory.h"
#include "PlaneTexture.h"



template<> ResourceManager* Singleton<ResourceManager>::singleton = nullptr;
ResourceManager::ResourceManager()
{
    stbi_set_flip_vertically_on_load(true);

    gltf_loader = std::make_unique<GLTFLoader>();
}
PlaneTexture* ResourceManager::CreatePlaneTexture(TextureType type, bool b_buffer, TextureFile* file)
{
    if (file != nullptr)
    {
        for (const auto& tex : plane_texture_array)
        {
            if (file == tex->file)
            {
                return tex;
            }
        }
    }

    PlaneTexture* texture = nullptr;
    switch (type)
    {
    case TextureType::EMPTY2D:
        texture = new PlaneTexture(type, file);
        break;
    case TextureType::EQUIRECTANGULARMAP:
        texture = new EquirectangularMap(type, file);
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
    case TextureType::ATTACHMENT:
        texture = new AttachmentTexture();
        break;
    default:
        break;
    }

    if (b_buffer)
    {
        texture->Buffer();
    }

    plane_texture_array.emplace(texture);

    return texture;
}



TextureFile* ResourceManager::CreateTextureFile(TextureFileType type, bool b_load, const string& path)
{
    for (const auto& file : texture_file_array)
    {
        if (path == file->path)
        {
            return file;
        }
    }

    TextureFile* texture_file = nullptr;
    switch (type)
    {
    case TextureFileType::LDR:
        texture_file = new LDRTextureFile(path);
    	break;
    case TextureFileType::HDR:
        texture_file = new HDRTextureFile(path);
        break;
    }

    if (b_load)
    {
        texture_file->LoadData();
    }

    texture_file_array.emplace(texture_file);

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
	material_array.emplace(material);
	return material;

}

Material* ResourceManager::CreateMaterial(const MaterialFactory& mat_factory)
{
    Material* material = mat_factory.CreateMaterial();
    material_array.emplace(material);
    return material;
}

Mesh* ResourceManager::CreateMesh(const MeshFactory& mesh_factory)
{

    Mesh* mesh = mesh_factory.CreateMesh();
    
    mesh->Initialize();

    mesh_array.emplace(mesh);
    return mesh;
}

TriangleMesh* ResourceManager::CreateTriangleMesh(bool b_buffer, const vector<ExpandedVertex>& vertex_array, const vector<unsigned int>& index_array)
{
    TriangleMesh* mesh = new TriangleMesh(vertex_array, index_array);
    if (b_buffer)
    {
        mesh->Initialize();
    }

    mesh_array.emplace(mesh);
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
    render_module_array.emplace(render_module);

    return render_module;
}

RenderUnit* ResourceManager::CreateRenderUnit(RenderModule* parent, Mesh* mesh, Material* material)
{
	RenderUnit* ru = new RenderUnit(parent, mesh, material);
    parent->AttachRenderUnit(ru);
    //parent->AddRenderUnit(ru);
	render_unit_array.emplace(ru);
	return ru;
}

CubeMap* ResourceManager::CreateCubemap(int width, int height, TextureType type, bool b_buffer, TextureFile* file)  //£¿ µ÷»»typeºÍb_bufferµÄË³Ðò
{
    CubeMap* cube_map = new CubeMap(width, height, type, file);
    if (b_buffer)
    {
        cube_map->Buffer();
    }
    return cube_map;
}



