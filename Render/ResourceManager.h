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
	set<Mesh*> mesh_array;

	//map<string, TriangleMesh*> tri_mesh_array;

	set<Shader*> shader_array;

	set<PlaneTexture*> plane_texture_array;

	set<Material*> material_array;

	set<CubeMap*> cube_map_array;

	set<TextureFile*> texture_file_array;
	
	set<RenderModule*> render_module_array;

	set<RenderUnit*> render_unit_array;



public:

	unique_ptr<GLTFLoader> gltf_loader;

	ResourceManager();

	//? �ӿ�������width��height
	PlaneTexture* CreatePlaneTexture(TextureType type, bool b_buffer = false, TextureFile* file = nullptr);	//b_bufferΪtrue����ʹ����ѡTextureType��Ĭ�ϲ������л���

	//PlaneTexture* CreateMetalnessRoughnessMap(PlaneTexture* metalness_map, PlaneTexture* roughness_map);

	TextureFile* CreateTextureFile(TextureFileType type, bool b_load = false, const string& path = "");

	Material* CreateMaterial(MaterialType type);

	Material* CreateMaterial(const MaterialFactory& mat_factory);

	Mesh* CreateMesh(const MeshFactory& mesh_factory);	//- ֻ��������Geometry mesh

	TriangleMesh* CreateTriangleMesh(bool b_buffer = false, const vector<ExpandedVertex>& vertex_array = vector<ExpandedVertex>(), const vector<unsigned int>& index_array = vector<unsigned int>());

	RenderModule* CreateRenderModule(RenderModule* parent, RenderModuleType type = RenderModuleType::DEFAULT);

	RenderUnit* CreateRenderUnit(RenderModule* parent, Mesh* mesh = nullptr, Material* material = nullptr);

	CubeMap* CreateCubemap(int width, int height, TextureType type = TextureType::CUBEMAP, bool b_buffer = false, TextureFile* file = nullptr);






};

