#include "Texture.h"
#include "File.h"
#include "TextureFile.h"

#include <glad/glad.h>
#include <iostream>

Texture::Texture(TextureType type_, TextureFile* file_) :
	type(type_), file(file_), b_genarate_mipmap(false), wrap_param(GL_CLAMP_TO_EDGE), min_filter_param(GL_LINEAR),
	internal_format(GL_RGB16F), data_format(GL_RGB), data_type(GL_FLOAT), width(512), height(512), texture_id(0)
{

}

void Texture::LoadData()
{
	file->LoadData();
}

bool Texture::Buffer()
{
	glGenTextures(1, &texture_id);

	if (!file)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, data_type, 0);
		
		if (b_genarate_mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_param);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//stbi_image_free(data);
		
		b_buffered = true;
	}
	else
	{
		//stbi_image_free(data);
		//std::cout << "ERROR<Texture> from Buffer: texture failed to load at path: " << path << endl;
		
		//b_buffered = false;
	}
	return false;
}
