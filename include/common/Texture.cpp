#include "Texture.h"
#include "File.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION    
#include <stb_image.h>
#include <iostream>

unsigned int Texture::LoadTexture(const std::string& name, const std::string& directory)
{
	std::string filename = directory + '/' + name;

	return LoadTexture(filename);

}

Texture::Texture(const string& path, TextureType type_)
{
	data = stbi_load(path.c_str(), &width, &height, &component_num, 0); //倒数第二个参数为图片原始的通道数，不是输出的通道数
	if (!data)
	{
		cout << "ERROR<Texture> Failed to load data\n";
	}
	type = type_;

	Buffer();
}

void Texture::Buffer()
{
	GLCall(glGenTextures(1, &id));

	if (data)
	{
		GLenum format;
		if (component_num == 1)
			format = GL_RED;
		else if (component_num == 3)
			format = GL_RGB;
		else if (component_num == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//stbi_image_free(data);
		
		b_buffered = true;
	}
	else
	{
		std::cout << "ERROR<Texture> from Buffer: texture failed to load at path: " << path << endl;
		b_buffered = false;
	}
}

unsigned int Texture::LoadTexture(const std::string& name)
{

	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	int width, height, num_components;

	//通过stb_image导入图片文件
	unsigned char* data = stbi_load(File::GetTexturePath(name).c_str(), &width, &height, &num_components, 0); //倒数第二个参数为图片原始的通道数，不是输出的通道数
	//最后一个参数为需求的通道数（共rgba四通道），0代表返回图片的所有通道

	if (data)
	{
		GLenum format;
		if (num_components == 1)
			format = GL_RED;
		else if (num_components == 3)
			format = GL_RGB;
		else if (num_components == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << name << std::endl;
		stbi_image_free(data);
	}

	return texture_id;
}