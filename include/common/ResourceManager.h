#pragma once
#include "Prerequisite.h"
#include "Singleton.h"
#include "Material.h"
#include "Texture.h"
#include "CubeMap.h"
#include "RenderModule.h"
#include "GLTFLoader.h"


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
	vector<Mesh*> mesh_array;

	//map<string, TriangleMesh*> tri_mesh_array;

	vector<Shader*> shader_array;

	vector<Texture*> texture_array;

	vector<Material*> material_array;

	vector<CubeMap*> cube_map_array;

	vector<TextureFile*> texture_file_array;
	
	vector<RenderModule*> render_module_array;

	vector<RenderUnit*> render_unit_array;

public:

	ResourceManager();

	Texture* CreateTexture(TextureType type, TextureFile* file = nullptr, bool b_buffer = false);

	Texture* CreateMetalnessRoughnessMap(Texture* metalness_map, Texture* roughness_map);

	TextureFile* CreateTextureFile(const string& path, TextureFileType type);

	Material* CreateMaterial(MaterialType type);

	Material* CreateMaterial(const MaterialFactory& mat_factory);

	Mesh* CreateMesh(const MeshFactory& mesh_factory);	//- 只用来生成Geometry mesh

	TriangleMesh* CreateTriangleMesh(bool b_buffer = false, const vector<ExpandedVertex>& vertex_array = vector<ExpandedVertex>(), const vector<unsigned int>& index_array = vector<unsigned int>());

	RenderModule* CreateRenderModule(RenderModule* parent, RenderModuleType type = RenderModuleType::DEFAULT);

	RenderUnit* CreateRenderUnit(RenderModule* parent, Mesh* mesh = nullptr, Material* material = nullptr);

	//CubeMap* CreateCubeMap(int width, int height);

	unique_ptr<GLTFLoader> gltf_loader;


};

