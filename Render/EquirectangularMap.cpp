#include "EquirectangularMap.h"

EquirectangularMap::EquirectangularMap(TextureType type_ /*= TextureType::EQUIRECTANGULARMAP*/, TextureFile* file_ /*= nullptr*/):
	PlaneTexture(type_, file_)
{
	wrap_param = GL_CLAMP_TO_EDGE;
	data_type = GL_FLOAT;
}

bool EquirectangularMap::Buffer()	//? 删除，改成调用基类的
{
	if (file)
	{

		if (dynamic_cast<HDRTextureFile*>(file))
		{
			HDRTextureFile* hdr_file = dynamic_cast<HDRTextureFile*>(file);
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, hdr_file->width, hdr_file->height, 0, GL_RGB, GL_FLOAT, hdr_file->data); // note how we specify the texture's data value to be float

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_param);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_param);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_param);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			b_buffered = true;
		}
		else
		{
			cout << "ERROR<Texture> Failed to buffer data\n";
		}
	}

	return b_buffered;
}
