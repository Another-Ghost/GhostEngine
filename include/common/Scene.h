#pragma once
#include "Datatype.h"
#include "Texture.h"
#include "Mesh.h"

#include <vector>



class Scene
{
public:
	Scene(const string& path)
	{
		LoadScene(path);
	}

private:
	const aiScene* scene;

	string directory;

	static std::vector<Texture> textures;
	
	//static std::vector<Mesh> meshes;

	void LoadScene(const string& path);

	void ProcessNode(aiNode* node);

	Mesh ProcessMesh(aiMesh* mesh);

	std::vector<Texture> ProcessMaterial(aiMaterial* mat);

	std::vector<Texture> ProcessTexture(aiMaterial* mat, aiTextureType ai_type, TextureType type);

	//static unsigned int LoadTexture(const string& name, const string& directory);	//? 在何处进行gamma校正


};

