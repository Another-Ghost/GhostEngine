#pragma once
#include "Mesh.h"
#include "shader.h"
#include "Texture.h"


struct ExpandedVertex {
	// position
	glm::vec3 position;
	// texCoords
	glm::vec2 tex_coords;
	// normal
	glm::vec3 normal;
	// tangent
	glm::vec3 tangent;
	// bitangent
	glm::vec3 bitangent;
};

class TriangleMesh : public Mesh
{
public:
	//string path;

	// mesh Data
	vector<ExpandedVertex>       vertex_array;
	vector<unsigned int> index_array;
	//vector<Texture>      textures;
	//unsigned int VAO;

	// constructor
	TriangleMesh(const vector<ExpandedVertex>& vertex_array_, const vector<unsigned int>& index_array_):
		vertex_array(vertex_array_), index_array(index_array_)
	{
		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		//Buffer();
	}

	void Buffer() override;

	void Draw(Shader* shader) override;

//	// render the mesh
//	void Draw(Shader& shader)
//	{
//		// bind appropriate textures
//		unsigned int diffuseNr = 1;
//		unsigned int specularNr = 1;
//		unsigned int normalNr = 1;
//		unsigned int heightNr = 1;
//		for (unsigned int i = 0; i < textures.size(); i++)
//		{
//			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
//			// retrieve texture number (the N in diffuse_textureN)
//			string number;
//			TextureType type = textures[i].type;
//			if (type == TextureType::DIFFUSE)
//				number = std::to_string(diffuseNr++);
//			else if (type == TextureType::SPECULAR)
//				number = std::to_string(specularNr++); // transfer unsigned int to stream
//			else if (type == TextureType::NORMAL)
//				number = std::to_string(normalNr++); // transfer unsigned int to stream
//			else if (type == TextureType::HEIGHT)
//				number = std::to_string(heightNr++); // transfer unsigned int to stream
//
//			// now set the sampler to the correct texture unit
//			glUniform1i(glGetUniformLocation(shader.GetID(), (Texture::TypeName(type) + number).c_str()), i);
//			// and finally bind the texture
//			glBindTexture(GL_TEXTURE_2D, textures[i].id);
//		}
//
//		// draw mesh
//		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//		// always good practice to set everything back to defaults once configured.
//		glActiveTexture(GL_TEXTURE0);
//	}
//
//private:
//	// render data 
//	//unsigned int VBO, EBO;
//
//	// initializes all the buffer objects/arrays
//	void setupMesh()
//	{
//		// create buffers/arrays
//		glGenVertexArrays(1, &VAO);
//		glGenBuffers(1, &VBO);
//		glGenBuffers(1, &EBO);
//
//		glBindVertexArray(VAO);
//		// load data into vertex buffers
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//		// A great thing about structs is that their memory layout is sequential for all its items.
//		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
//		// again translates to 3/2 floats which translates to a byte array.
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ExpandedVertex), &vertices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//		// set the vertex attribute pointers
//		// vertex Positions
//		glEnableVertexAttribArray(0); //参数对应于shader中的position
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)0);
//		// vertex normals
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)offsetof(ExpandedVertex, Normal));
//		// vertex texture coords
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)offsetof(ExpandedVertex, TexCoords));
//		// vertex tangent
//		glEnableVertexAttribArray(3);
//		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)offsetof(ExpandedVertex, Tangent));
//		// vertex bitangent
//		glEnableVertexAttribArray(4);
//		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ExpandedVertex), (void*)offsetof(ExpandedVertex, Bitangent));
//
//		glBindVertexArray(0);
//	}

};

