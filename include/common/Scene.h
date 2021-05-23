#pragma once
#include "Datatype.h"
#include "Texture.h"
#include "Mesh.h"

#include <vector>



class Scene
{
	static std::vector<Texture> textures;
	
	//static std::vector<Mesh> meshes;

	Scene(const string& path)
	{
		LoadScene(path);
	}

	void LoadScene(const string& path);

	void ProcessNode(aiNode* node);

	Mesh ProcessMesh(aiMesh* mesh);

	std::vector<Texture> ProcessMaterial(aiMaterial* mat);

	std::vector<Texture> ProcessTexture(aiMaterial* mat, aiTextureType ai_type, TextureType type);

	unsigned int LoadTexture(const string& name, const string& directory);	//? 在何处进行gamma校正

private:
	const aiScene* scene;

	string directory;
};

