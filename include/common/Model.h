#pragma once
#include "Datatype.h"
#include "Prerequisite.h"

#include <vector>



class Model
{
public:
	Model(const string& path);


private:
	const aiScene* scene;

	//string directory;

	//static std::vector<Texture> textures;
	
	//RootRenderModule* root_render_module;
	//static std::vector<Mesh> meshes;
	map<int, Material*> material_map;


	void LoadScene(const string& path);

	BasicMaterial* ProcessMaterial(aiMaterial* ai_mat);

	vector<Texture*> ProcessTexture(aiMaterial* mat, aiTextureType ai_type, TextureType type);

	void ProcessNode(aiNode* node, RenderModule* render_module);

	void ProcessMesh(aiMesh* ai_mesh, RenderUnit* render_unit);

	//std::vector<Texture> ProcessMaterial(aiMaterial* mat);

	//std::vector<Texture> ProcessTexture(aiMaterial* mat, aiTextureType ai_type, TextureType type);

	//static unsigned int LoadTexture(const string& name, const string& directory);	//? 在何处进行gamma校正


};