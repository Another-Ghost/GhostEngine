#include "Scene.h"
#include "Mesh.h"
#include "File.h"
#include <stb_image.h>
#include <iostream>

void Scene::LoadScene(const string& path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	scene = importer.ReadFile(File::GetScenePath(path), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}

	// retrieve the directory path of the filepath
	directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively
	ProcessNode(scene->mRootNode);
}

void Scene::ProcessNode(aiNode* node)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh);
		//meshes.push_back(ProcessMesh(mesh)); //这种存储方式并没有保留网格间的父子关系, 试着自己实现下另一种
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	//深度优先遍历
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i]);
	}
}

Mesh Scene::ProcessMesh(aiMesh* mesh)
{
	// data to fill
	vector<Vertex> vertices;
	vector<unsigned int> indices;


	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		
		// positions
		vertex.Position = Datatype::ToGLMVec3(mesh->mVertices[i]);
	
		// normals
		if (mesh->HasNormals())
			vertex.Normal = Datatype::ToGLMVec3(mesh->mNormals[i]);
		
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

			// tangent
			vertex.Tangent = Datatype::ToGLMVec3(mesh->mNormals[i]);
			
			// bitangent
			vertex.Bitangent = Datatype::ToGLMVec3(mesh->mBitangents[i]);
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	
	vector<Texture> texs = ProcessMaterial(material);

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, texs);
}

std::vector<Texture> Scene::ProcessMaterial(aiMaterial* mat)
{
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN
	std::vector<Texture> texs;
	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = ProcessTexture(mat, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
	texs.insert(texs.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = ProcessTexture(mat, aiTextureType_SPECULAR, TextureType::SPECULAR);
	texs.insert(texs.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = ProcessTexture(mat, aiTextureType_NORMALS, TextureType::NORMAL);
	texs.insert(texs.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = ProcessTexture(mat, aiTextureType_HEIGHT, TextureType::HEIGHT);
	texs.insert(texs.end(), heightMaps.begin(), heightMaps.end());

	return texs;
}




std::vector<Texture> Scene::ProcessTexture(aiMaterial* mat, aiTextureType ai_type, TextureType type)
{
	vector<Texture> texs;
	for (unsigned int i = 0; i < mat->GetTextureCount(ai_type); i++)
	{
		aiString str;
		mat->GetTexture(ai_type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures.size(); ++j)
		{
			if (std::strcmp(textures[j].path.data(), str.C_Str()) == 0)   //插入的复杂度较高，但按id查询复杂度为O(1)
			{
				texs.push_back(textures[j]); //-只是跳过纹理加载/生成的部分，节省时间；因为元素为Texture结构体同样节省空间
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = Texture::LoadTexture(str.C_Str(), this->directory);
			texture.type = type;
			texture.path = str.C_Str();
			texs.push_back(texture);
			textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return texs;
}

//unsigned int Scene::LoadTexture(const string& name, const string& directory)
//{
//	string filename = directory + '/' + name;
//
//	unsigned int texture_id;
//	glGenTextures(1, &texture_id);
//
//	int width, height, num_components;
//
//	//通过stb_image导入图片文件
//	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &num_components, 0); //倒数第二个参数为图片原始的通道数，不是输出的通道数
//	//最后一个参数为需求的通道数（共rgba四通道），0代表返回图片的所有通道
//
//	if (data)
//	{
//		GLenum format;
//		if (num_components == 1)
//			format = GL_RED;
//		else if (num_components == 3)
//			format = GL_RGB;
//		else if (num_components == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, texture_id);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << name << std::endl;
//		stbi_image_free(data);
//	}
//
//	return texture_id;
//}