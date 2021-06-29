#include "MaterialMap.h"
#include "TextureFile.h"

MaterialMap::MaterialMap(TextureType type_, TextureFile* file_, bool b_buffer):
	Texture(type_, file_)
{
	wrap_param = GL_REPEAT;
	b_genarate_mipmap = true;
	min_filter_param = GL_LINEAR_MIPMAP_LINEAR;

}

bool MaterialMap::Buffer()
{
	if (file)
	{
		if (!file->b_loaded)
			file->LoadData();

		if (dynamic_cast<LDRTextureFile*>(file))
		{
			LDRTextureFile* ldr_file = dynamic_cast<LDRTextureFile*>(file);
			glGenTextures(1, &texture_id);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexImage2D(GL_TEXTURE_2D, 0, ldr_file->format, ldr_file->width, ldr_file->height, 0, ldr_file->format, GL_UNSIGNED_BYTE, ldr_file->data);

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

	}
	return b_buffered;
}
