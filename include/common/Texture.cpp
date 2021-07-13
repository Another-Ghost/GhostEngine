#include "Texture.h"
#include "File.h"
#include "TextureFile.h"

#include <glad/glad.h>
#include <iostream>

Texture::Texture(TextureType type_, TextureFile* file_) :
	type(type_), file(file_), b_genarate_mipmap(false), wrap_param(GL_CLAMP_TO_EDGE), min_filter_param(GL_LINEAR),
	internal_format(GL_RGB16F), data_format(GL_RGB), data_type(GL_FLOAT), width(512), height(512), texture_id(0)
{
	//因为constructor不应该调用virtual方法，所有不能调用Buffer
}

void Texture::LoadData()
{
	file->LoadData();
}

bool Texture::Buffer()
{
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	if (!file)
	{

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, data_type, 0);

		//stbi_image_free(data);
	}
	else
	{
		if (!file->b_loaded)
			file->LoadData();

		internal_format = file->format;
		width = file->width;
		height = file->height;
		data_format = file->format;

		if (dynamic_cast<LDRTextureFile*>(file))
		{
			LDRTextureFile* ldr_file = dynamic_cast<LDRTextureFile*>(file);
			glTexImage2D(GL_TEXTURE_2D, 0, ldr_file->format, ldr_file->width, ldr_file->height, 0, ldr_file->format, GL_UNSIGNED_BYTE, ldr_file->data);
		}
		//stbi_image_free(data);
		//std::cout << "ERROR<Texture> from Buffer: texture failed to load at path: " << path << endl;
		
		//b_buffered = false;
	}

	if (b_genarate_mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	b_buffered = true;

	return b_buffered;
}
