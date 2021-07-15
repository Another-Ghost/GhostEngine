#include "GLTFLoader.h"
#include "RootRenderModule.h"
#include "RenderUnit.h"
#include "ResourceManager.h"
#include "PBRMaterial.h"
#include "Texture.h"
#include "TriangleMesh.h"
#include "TextureFile.h"

//using namespace Microsoft::glTF;

RootRenderModule* GLTFLoader::LoadFile(const string& path_str)
{
	material_map.clear();
	mesh_map.clear();

	std::filesystem::path path = path_str;
	parent_path = path.parent_path();

	//if (path.is_relative())
	//{
	//	auto pathCurrent = std::filesystem::current_path();

	//	// Convert the relative path into an absolute path by appending the command line argument to the current path
	//	pathCurrent /= path;
	//	pathCurrent.swap(path);
	//}
	auto stream_reader = std::make_unique<StreamReader>(path.parent_path());

	std::filesystem::path path_file = path.filename();
	std::filesystem::path path_file_ext = path_file.extension();

	std::string manifest;

	auto MakePathExt = [](const std::string& ext)
	{
		return "." + ext;
	};

	std::unique_ptr<Microsoft::glTF::GLTFResourceReader> resource_reader;

	// If the file has a '.gltf' extension then create a GLTFResourceReader
	if (path_file_ext == MakePathExt(Microsoft::glTF::GLTF_EXTENSION))
	{
		auto gltf_stream = stream_reader->GetInputStream(path_file.u8string()); // Pass a UTF-8 encoded filename to GetInputString
		auto gltf_resource_reader = std::make_unique<Microsoft::glTF::GLTFResourceReader>(std::move(stream_reader));

		std::stringstream manifest_stream;

		// Read the contents of the glTF file into a string using a std::stringstream
		manifest_stream << gltf_stream->rdbuf();
		manifest = manifest_stream.str();

		resource_reader = std::move(gltf_resource_reader);	//显示的转移unique_ptr的控制权
	}

	// If the file has a '.glb' extension then create a GLBResourceReader. This class derives
	// from GLTFResourceReader and adds support for reading manifests from a GLB container's
	// JSON chunk and resource data from the binary chunk.
	if (path_file_ext == MakePathExt(Microsoft::glTF::GLB_EXTENSION))
	{
		auto glb_stream = stream_reader->GetInputStream(path_file.u8string()); // Pass a UTF-8 encoded filename to GetInputString
		auto glb_resource_reader = std::make_unique<Microsoft::glTF::GLBResourceReader>(std::move(stream_reader), std::move(glb_stream));

		manifest = glb_resource_reader->GetJson(); // Get the manifest from the JSON chunk

		resource_reader = std::move(glb_resource_reader);
	}

	if (!resource_reader)
	{
		throw std::runtime_error("ERROR<Loader>: path filename extension must be .gltf or .glb");
	}

	Microsoft::glTF::Document document;

	try
	{
		document = Microsoft::glTF::Deserialize(manifest);
	}
	catch (const Microsoft::glTF::GLTFException& ex)
	{
		std::stringstream ss;

		ss << "ERROR<Loader>: Microsoft::glTF::Deserialize failed: ";
		ss << ex.what();

		throw std::runtime_error(ss.str());
	}

	if (document.scenes.Size() > 0U)
	{
		material_map = ProcessMaterial(document, *resource_reader);
		mesh_map = ProcessMesh(document, *resource_reader);

		std::cout << "Default Scene Index: " << document.GetDefaultScene().id << "\n\n";
		return ProcessScene(document, *resource_reader);
	}

	return nullptr;
}

RootRenderModule* GLTFLoader::ProcessScene(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resource_reader)
{
	RootRenderModule* root_render_module = nullptr;
	for (const auto& node_id : document.GetDefaultScene().nodes)	//scene只保留node的id
	{
		root_render_module = dynamic_cast<RootRenderModule*>(ResourceManager::GetSingleton().CreateRenderModule(nullptr, RenderModuleType::ROOT));

		ProcessNode(node_id, root_render_module, document, resource_reader);
	}

	return root_render_module;
}

void GLTFLoader::ProcessNode(const string& node_id, RenderModule* render_module, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resource_reader)
{
	RenderUnit* render_unit = ResourceManager::GetSingleton().CreateRenderUnit(render_module);

	const auto& node = document.nodes.Get(node_id);
	const auto& mesh = document.meshes.Get(node.meshId);
	render_unit->SetMesh(mesh_map[mesh.id]);
	
	for (const auto& meshPrimitive : mesh.primitives)
	{
		render_unit->SetMaterial(material_map[meshPrimitive.materialId]);
	}
	
	for (const auto& child_node_id : node.children)
	{
		RenderModule* child_rm = ResourceManager::GetSingleton().CreateRenderModule(render_module);
		ProcessNode(node_id, child_rm, document, resource_reader);
	}

	//ProcessMesh(document.nodes.Get(node_id).meshId, render_module, document, resource_reader);
}


map<string, Material*> GLTFLoader::ProcessMaterial(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resource_reader)
{
	map<string, Material*> material_map;
	for (const auto& gltf_mat : document.materials.Elements())
	{
		PBRMaterial* material = dynamic_cast<PBRMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::PBR));

		string img_path = parent_path.string() + "/" + document.images.Get(document.textures.Get(gltf_mat.metallicRoughness.baseColorTexture.textureId).imageId).uri;
		LDRTextureFile* tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, img_path));
		material->basecolor_map = ResourceManager::GetSingleton().CreateTexture(TextureType::BASECOLOR, true, tex_file);
		//material->basecolor_map->wrap_param = GL_CLAMP;
		//material->basecolor_map->min_filter_param = GL_LINEAR;
		//material->basecolor_map->b_genarate_mipmap = false;
		//material->basecolor_map->Buffer();
		if (gltf_mat.normalTexture.textureId != "")
		{
			img_path = parent_path.string() + "/" + document.images.Get(document.textures.Get(gltf_mat.normalTexture.textureId).imageId).uri;
			tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, img_path));
			material->normal_map = ResourceManager::GetSingleton().CreateTexture(TextureType::NORMAL, true, tex_file);
			//material->normal_map->wrap_param = GL_CLAMP;
			//material->normal_map->Buffer();
		}

		if (gltf_mat.occlusionTexture.textureId != "")
		{
			img_path = parent_path.string() + "/" + document.images.Get(document.textures.Get(gltf_mat.occlusionTexture.textureId).imageId).uri;
			tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, img_path));
			material->ao_map = ResourceManager::GetSingleton().CreateTexture(TextureType::AO, true, tex_file);
			//material->ao_map->wrap_param = GL_CLAMP;
			//material->ao_map->Buffer();
		}

		if (gltf_mat.metallicRoughness.metallicRoughnessTexture.textureId != "")
		{
			img_path = parent_path.string() + "/" + document.images.Get(document.textures.Get(gltf_mat.metallicRoughness.metallicRoughnessTexture.textureId).imageId).uri;
			tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, img_path));
			material->metalness_roughness_map = ResourceManager::GetSingleton().CreateTexture(TextureType::METALNESSROUGHNESS, true, tex_file);
			//material->metalness_roughness_map->wrap_param = GL_CLAMP;
			//material->metalness_roughness_map->Buffer();
		}


		if (gltf_mat.emissiveTexture.textureId != "")
		{
			img_path = parent_path.string() + "/" + document.images.Get(document.textures.Get(gltf_mat.emissiveTexture.textureId).imageId).uri;
			tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, img_path));
			material->emissive_map = ResourceManager::GetSingleton().CreateTexture(TextureType::METALNESSROUGHNESS, true, tex_file);
		}

		material_map.emplace(gltf_mat.id, material);
	}
	
	return material_map;
}

map<string, Mesh*> GLTFLoader::ProcessMesh(const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resource_reader)
{
	map<string, Mesh*> mesh_map;

	//TriangleMesh* mesh = ResourceManager::GetSingleton().CreateTriangleMesh();
	//glGenVertexArrays(1, &mesh->vao_id);
	//glGenBuffers(1, &mesh->vbo_id);
	//glGenBuffers(1, &mesh->ebo_id);
	//glBindVertexArray(mesh->vao_id);
	//glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_id);


	for (const auto& gltf_mesh : document.meshes.Elements())
	{

		vector<ExpandedVertex> vertex_array;
		vector<unsigned int>  index_array;
		for (const auto& mesh_primitive : gltf_mesh.primitives)
		{
			int byte_count = 0;
			std::string accessor_id;
			if (mesh_primitive.TryGetAttributeAccessorId(Microsoft::glTF::ACCESSOR_POSITION, accessor_id))
			{
				const Microsoft::glTF::Accessor& accessor = document.accessors.Get(accessor_id);
				const auto data = resource_reader.ReadBinaryData<float>(document, accessor);
				//glBufferData(GL_ARRAY_BUFFER, accessor.count * sizeof(float) * 12, 0, GL_STATIC_DRAW);
				//glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), &data[0]);
				//glEnableVertexAttribArray(0);
				//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
				//byte_count += data.size() * sizeof(float);

				vertex_array.resize(accessor.count);

				for (unsigned int i = 0; i < accessor.count; ++i)
				{
					int num = i * 3;
					vertex_array[i].position = vec3(data[num], data[num + 1], data[num + 2]);
				}
			}

			if (mesh_primitive.TryGetAttributeAccessorId(Microsoft::glTF::ACCESSOR_TEXCOORD_0, accessor_id))
			{
				const Microsoft::glTF::Accessor& accessor = document.accessors.Get(accessor_id);
				const auto data = resource_reader.ReadBinaryData<float>(document, accessor);

				//glBufferSubData(GL_ARRAY_BUFFER, byte_count, data.size() * sizeof(float), &data[0]);
				//glEnableVertexAttribArray(1);
				//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)byte_count);
				//byte_count += data.size() * sizeof(float);

				for (unsigned int i = 0; i < accessor.count; ++i)
				{
					int num = i * 2;
					vertex_array[i].tex_coords = { data[num], -data[num + 1] };
				}
			}

			if (mesh_primitive.TryGetAttributeAccessorId(Microsoft::glTF::ACCESSOR_NORMAL, accessor_id))
			{
				const Microsoft::glTF::Accessor& accessor = document.accessors.Get(accessor_id);
				const auto data = resource_reader.ReadBinaryData<float>(document, accessor);

				//glBufferSubData(GL_ARRAY_BUFFER, byte_count, data.size() * sizeof(float), &data[0]);
				//glEnableVertexAttribArray(2);
				//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)byte_count);
				//byte_count += data.size() * sizeof(float);

				for (unsigned int i = 0; i < accessor.count; ++i)
				{
					int num = i * 3;
					vertex_array[i].normal = { data[num], data[num + 1], data[num + 2] };
				}
			}

			if (mesh_primitive.TryGetAttributeAccessorId(Microsoft::glTF::ACCESSOR_TANGENT, accessor_id))
			{
				const Microsoft::glTF::Accessor& accessor = document.accessors.Get(accessor_id);
				const auto data = resource_reader.ReadBinaryData<float>(document, accessor);

				//glBufferSubData(GL_ARRAY_BUFFER, byte_count, data.size() * sizeof(float), &data[0]);
				//glEnableVertexAttribArray(3);
				//glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)byte_count);
				//byte_count += data.size() * sizeof(float);

				for (unsigned int i = 0; i < accessor.count; ++i)
				{
					int num = i * 4;
					vertex_array[i].tangent = { data[num], data[num + 1], data[num + 2] };
				}
			}
			//bitangent = cross(normal, tangent.xyz) * tangent.w

			const Microsoft::glTF::Accessor& accessor = document.accessors.Get(mesh_primitive.indicesAccessorId);
			const auto data = resource_reader.ReadBinaryData<unsigned short>(document, accessor);

			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo_id);
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_STATIC_DRAW);
			//glBindVertexArray(0);

			index_array.resize(accessor.count);
			for (unsigned int i = 0; i < accessor.count; ++i)
			{
				index_array[i] = data[i];
			}
		}
		//mesh->vertex_array = std::move(vertex_array);
		//mesh->index_array = std::move(index_array);

		TriangleMesh* mesh = ResourceManager::GetSingleton().CreateTriangleMesh(true, std::move(vertex_array), std::move(index_array));

		mesh_map.emplace(gltf_mesh.id, mesh);
	}

	return mesh_map;
}




//RenderUnit* GLTFLoader::ProcessMesh(const string& mesh_id, RenderModule* parent_rm, const Microsoft::glTF::Document& document, const Microsoft::glTF::GLTFResourceReader& resource_reader)
//{
//	RenderUnit* render_unit = ResourceManager::GetSingleton().CreateRenderUnit(parent_rm);	//空白ru
//	document.meshes[mesh_id].
//
//}


