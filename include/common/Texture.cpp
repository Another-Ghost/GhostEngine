#include "Texture.h"
#include "File.h"
#include "TextureFile.h"

#include <glad/glad.h>
#include <iostream>

Texture::Texture(TextureType type_, TextureFile* file_) :
	type(type_), file(file_), b_genarate_mipmap(false), wrap_param(GL_CLAMP_TO_EDGE), min_filter_param(GL_LINEAR), mag_filter_param(GL_LINEAR),
	internal_format(GL_RGB16F), data_format(GL_RGB), data_type(GL_UNSIGNED_BYTE), width(512), height(512), id(0)	//? wrap_param改成gl_repeat
{
	//因为constructor不应该调用virtual方法，所有不能调用Buffer
}

void Texture::LoadData()
{
	file->LoadData();
}

bool Texture::Buffer()
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	if (!file)
	{

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, data_type, 0);

		//stbi_image_free(data);
	}
	else
	{

		//internal_format = file->format;
		width = file->width;
		height = file->height;
		data_format = file->format;

		if (dynamic_cast<LDRTextureFile*>(file))
		{
			LDRTextureFile* ldr_file = dynamic_cast<LDRTextureFile*>(file);
			//glTexImage2D(GL_TEXTURE_2D, 0, ldr_file->format, ldr_file->width, ldr_file->height, 0, ldr_file->format, GL_UNSIGNED_BYTE, ldr_file->data);
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format, ldr_file->width, ldr_file->height, 0, data_format, GL_UNSIGNED_BYTE, ldr_file->data);

		}
		else if (dynamic_cast<HDRTextureFile*>(file))
		{
			HDRTextureFile* hdr_file = dynamic_cast<HDRTextureFile*>(file);
			//glTexImage2D(GL_TEXTURE_2D, 0, ldr_file->format, ldr_file->width, ldr_file->height, 0, ldr_file->format, GL_UNSIGNED_BYTE, ldr_file->data);
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format, hdr_file->width, hdr_file->height, 0, data_format, GL_FLOAT, hdr_file->data);
		}
		else
		{
			cout << "ERROR<Texture>: Texture data loading failed \n";
			return b_buffered;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_param);

	glBindTexture(GL_TEXTURE_2D, 0);

	b_buffered = true;

	return b_buffered;
}
