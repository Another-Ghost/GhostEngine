#include "Model.h"
#include "RootRenderModule.h"
#include "ResourceManager.h"
#include "BasicMaterial.h"
#include "Texture.h"
#include "TriangleMeshFactory.h"
#include "RenderUnit.h"
#include "PBRMaterial.h"

Model::Model()
{

}


RootRenderModule* Model::LoadScene(const string& path, MaterialType mat_type)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR<Assimp>: " << importer.GetErrorString() << endl;
		return nullptr;
	}
	//path = "D:/Codes/Another-Ghost/GhostEngine/resource/object/backpack/backpack.obj"
			//D:\Codes\Another-Ghost\GhostEngine\resource\object\backpack
	//path = "D:/Codes/Another-Ghost/LearnOpenGL/resources/objects/backpack/backpack.obj"
	directory = path.substr(0, path.find_last_of('/')) + '/';

	material_map.clear();
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		Material* material = ProcessMaterial(scene->mMaterials[i], mat_type);
		material_map.emplace(i, material);

		int n = scene->mMaterials[i]->GetTextureCount(aiTextureType_NONE);
		cout << "aiTextureType_NONE" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
		cout << "aiTextureType_DIFFUSE" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR);
		cout << "aiTextureType_SPECULAR" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_AMBIENT);
		cout << "aiTextureType_AMBIENT" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_EMISSIVE);
		cout << "aiTextureType_EMISSIVE" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_HEIGHT);
		cout << "aiTextureType_HEIGHT" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMALS);
		cout << "aiTextureType_NORMALS" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_SHININESS);
		cout << "aiTextureType_SHININESS" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_OPACITY);
		cout << "aiTextureType_OPACITY" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_DISPLACEMENT);
		cout << "aiTextureType_DISPLACEMENT" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_LIGHTMAP);
		cout << "aiTextureType_LIGHTMAP" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_REFLECTION);
		cout << "aiTextureType_REFLECTION" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_BASE_COLOR);
		cout << "aiTextureType_BASE_COLOR" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_NORMAL_CAMERA);
		cout << "aiTextureType_NORMAL_CAMERA" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_EMISSION_COLOR);
		cout << "aiTextureType_EMISSION_COLOR" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_METALNESS);
		cout << "aiTextureType_METALNESS" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
		cout << "aiTextureType_DIFFUSE_ROUGHNESS" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
		cout << "aiTextureType_AMBIENT_OCCLUSION" << ": " << n << "\n";
		n = scene->mMaterials[i]->GetTextureCount(aiTextureType_UNKNOWN);
		cout << "aiTextureType_UNKNOWN" << ": " << n << "\n";
	}

	//for (int i = 0; i < scene->mNumMeshes; ++i)
	//{
	//	ProcessMesh(scene->mMeshes[i]);
	//}

	// retrieve the directory path of the filepath

	// process ASSIMP's root node recursively
	RootRenderModule* root_render_module = dynamic_cast<RootRenderModule*>(ResourceManager::GetSingleton().CreateRenderModule(nullptr, RenderModuleType::ROOT));
	ProcessNode(scene->mRootNode, root_render_module);

	return root_render_module;
}

Material* Model::ProcessMaterial(aiMaterial* ai_mat, MaterialType mat_type)
{

	if (mat_type == MaterialType::BASIC)
	{
		BasicMaterial* mat = dynamic_cast<BasicMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::BASIC));

		// 1. diffuse maps
		mat->diffuse_map_array = ProcessTexture(ai_mat, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
		// 2. specular maps
		mat->specular_map_array = ProcessTexture(ai_mat, aiTextureType_SPECULAR, TextureType::SPECULAR);
		// 3. normal maps
		mat->normal_map_array = ProcessTexture(ai_mat, aiTextureType_NORMALS, TextureType::NORMAL);

		return mat;
	}

	if (mat_type == MaterialType::PBR)
	{
		PBRMaterial* mat = dynamic_cast<PBRMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::PBR));
		vector<Texture*> tex_array = ProcessTexture(ai_mat, aiTextureType_BASE_COLOR, TextureType::BASECOLOR);
		mat->basecolor_map = tex_array.empty() ? nullptr : tex_array.front();

		tex_array = ProcessTexture(ai_mat, aiTextureType_NORMALS, TextureType::NORMAL);
		mat->normal_map = tex_array.empty() ? nullptr : tex_array.front();
		tex_array = ProcessTexture(ai_mat, aiTextureType_METALNESS, TextureType::METALNESS);
		mat->metalness_map = tex_array.empty() ? nullptr : tex_array.front();
		tex_array = ProcessTexture(ai_mat, aiTextureType_DIFFUSE_ROUGHNESS, TextureType::ROUGHNESS);
		mat->roughness_map = tex_array.empty() ? nullptr : tex_array.front();
		tex_array = ProcessTexture(ai_mat, aiTextureType_AMBIENT_OCCLUSION, TextureType::AO);
		mat->ao_map = tex_array.empty() ? nullptr : tex_array.front();
		
		return mat;
	}

	return nullptr;
}

vector<Texture*> Model::ProcessTexture(aiMaterial* mat, aiTextureType ai_type, TextureType type)
{
	vector<Texture*> tex_array;
	int n = mat->GetTextureCount(ai_type);
	cout << Texture::TypeName(type) << ": " << n << "\n";
	for (unsigned int i = 0; i < mat->GetTextureCount(ai_type); i++)
	{
		aiString str;
		mat->GetTexture(ai_type, i, &str);

		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		TextureFile* tex_file = ResourceManager::GetSingleton().CreateTextureFile(directory + str.C_Str(), TextureFileType::LDR);
		Texture* tex = ResourceManager::GetSingleton().CreateTexture(type, tex_file, true);

		//tex->b_genarate_mipmap = true;
		//tex->wrap_param = GL_REPEAT;
		//tex->min_filter_param = GL_LINEAR_MIPMAP_LINEAR;

		//tex->Buffer();
		tex_array.emplace_back(tex);
	}

	return tex_array;
}

void Model::ProcessNode(aiNode* node, RenderModule* render_module)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		RenderUnit* render_unit = ResourceManager::GetSingleton().CreateRenderUnit(render_module);	//空白ru

		ProcessMesh(mesh, render_unit);

		render_module->AddRenderUnit(render_unit);
		//meshes.push_back(ProcessMesh(mesh)); //这种存储方式并没有保留网格间的父子关系, 试着自己实现下另一种
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	//深度优先遍历
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		RenderModule* child_rm = ResourceManager::GetSingleton().CreateRenderModule(render_module);
		ProcessNode(node->mChildren[i], child_rm);
	}
}

void Model::ProcessMesh(aiMesh* ai_mesh, RenderUnit* render_unit)
{
	//TriangleMesh* mesh = ResourceManager::GetSingleton().CreateTriangleMesh()
	// data to fill
	vector<ExpandedVertex> vertex_array;
	vector<unsigned int> index_array;

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
	{
		ExpandedVertex vertex;
		//glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		
		// positions
		vertex.position = Datatype::ToGLVec3(ai_mesh->mVertices[i]);
	
		// normals
		if (ai_mesh->HasNormals())
			vertex.normal = Datatype::ToGLVec3(ai_mesh->mNormals[i]);
		
		// texture coordinates
		if (ai_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vertex.tex_coords = glm::vec2(ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y);

			// tangent
			vertex.tangent = Datatype::ToGLVec3(ai_mesh->mTangents[i]);
			
			// bitangent
			vertex.bitangent = Datatype::ToGLVec3(ai_mesh->mBitangents[i]);
		}
		else
			vertex.tex_coords = glm::vec2(0.0f, 0.0f);

		vertex_array.emplace_back(vertex);
	}
	// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++)
	{
		aiFace face = ai_mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			index_array.push_back(face.mIndices[j]);
	}
	// process materials
	// 
	//aiMaterial* ai_material = scene->mMaterials[ai_mesh->mMaterialIndex];
	
	Material* material = material_map[ai_mesh->mMaterialIndex];

	// return a mesh object created from the extracted mesh data
	TriangleMesh* mesh = ResourceManager::GetSingleton().CreateTriangleMesh(true, vertex_array, index_array);

	render_unit->SetMaterial(material);
	render_unit->SetMesh(mesh);
}

//std::vector<Texture> Model::ProcessMaterial(aiMaterial* mat)
//{
//	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
//	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
//	// Same applies to other texture as the following list summarizes:
//	// diffuse: texture_diffuseN
//	// specular: texture_specularN
//	// normal: texture_normalN
//	std::vector<Texture> texs;
//	// 1. diffuse maps
//	std::vector<Texture> diffuseMaps = ProcessTexture(mat, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
//	texs.insert(texs.end(), diffuseMaps.begin(), diffuseMaps.end());
//	// 2. specular maps
//	std::vector<Texture> specularMaps = ProcessTexture(mat, aiTextureType_SPECULAR, TextureType::SPECULAR);
//	texs.insert(texs.end(), specularMaps.begin(), specularMaps.end());
//	// 3. normal maps
//	std::vector<Texture> normalMaps = ProcessTexture(mat, aiTextureType_NORMALS, TextureType::NORMAL);
//	texs.insert(texs.end(), normalMaps.begin(), normalMaps.end());
//	// 4. height maps
//	std::vector<Texture> heightMaps = ProcessTexture(mat, aiTextureType_HEIGHT, TextureType::HEIGHT);
//	texs.insert(texs.end(), heightMaps.begin(), heightMaps.end());
//
//	return texs;
//}




//std::vector<Texture> Model::ProcessTexture(aiMaterial* mat, aiTextureType ai_type, TextureType type)
//{
//	vector<Texture> texs;
//	for (unsigned int i = 0; i < mat->GetTextureCount(ai_type); i++)
//	{
//		aiString str;
//		mat->GetTexture(ai_type, i, &str);
//		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//		bool skip = false;
//		for (unsigned int j = 0; j < textures.size(); ++j)
//		{
//			if (std::strcmp(textures[j].path.data(), str.C_Str()) == 0)   //插入的复杂度较高，但按id查询复杂度为O(1)
//			{
//				texs.push_back(textures[j]); //-只是跳过纹理加载/生成的部分，节省时间；因为元素为Texture结构体同样节省空间
//				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
//				break;
//			}
//		}
//		if (!skip)
//		{   // if texture hasn't been loaded already, load it
//			Texture texture;
//			texture.id = Texture::LoadTexture(str.C_Str(), this->directory);
//			texture.type = type;
//			texture.path = str.C_Str();
//			texs.push_back(texture);
//			textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//		}
//	}
//	return texs;
//}

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
