#include "Texture.h"
#include "File.h"
#include "TextureFile.h"

#include <glad/glad.h>
#include <iostream>

Texture::Texture(TextureType type_, TextureFile* file_) :
	type(type_), file(file_), b_genarate_mipmap(false), wrap_param(GL_REPEAT), min_filter_param(GL_LINEAR), mag_filter_param(GL_LINEAR),
	internal_format(GL_RGB16F), data_format(GL_RGB), data_type(GL_UNSIGNED_BYTE), width(512), height(512), id(0)	//? wrap_param改成gl_repeat
{
	//因为constructor不应该调用virtual方法，所有不能调用Buffer
}

