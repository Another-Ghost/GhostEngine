#pragma once
#include "Datatype.h"
#include "Prerequisite.h"
#include "Material.h"
#include <vector>



class Model
{
public:
	Model();

	RootRenderModule* LoadScene(const string& path, MaterialType mat_type = MaterialType::BASIC);

private:
	const aiScene* scene;

	string directory;

	//static std::vector<Texture> textures;
	
	//RootRenderModule* root_render_module;
	//static std::vector<Mesh> meshes;
	map<int, Material*> material_map;

	Material* ProcessMaterial(aiMaterial* ai_mat, MaterialType mat_type);

	vector<Texture*> ProcessTexture(aiMaterial* mat, aiTextureType ai_type, TextureType type);

	void ProcessNode(aiNode* node, RenderModule* render_module);

	void ProcessMesh(aiMesh* ai_mesh, RenderUnit* render_unit);

	//std::vector<Texture> ProcessMaterial(aiMaterial* mat);

	//std::vector<Texture> ProcessTexture(aiMaterial* mat, aiTextureType ai_type, TextureType type);

	//static unsigned int LoadTexture(const string& name, const string& directory);	//? 在何处进行gamma校正


};