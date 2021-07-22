#include "CubeMap.h"



CubeMap::CubeMap(int width_, int height_, TextureType type_, TextureFile* file_):
	Texture(type_, file_)
{
	width = width_;
	height = height_;
}

bool CubeMap::Buffer()
{
	if (!file) 
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, width, height, 0, data_format, data_type, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_param);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_param);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_param);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter_param);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (b_genarate_mipmap)
		{
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
		b_buffered = true;
	}
	return b_buffered;
}
