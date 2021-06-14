#include "TextureFile.h"
#define STB_IMAGE_IMPLEMENTATION    
#include <stb_image.h>

map<int, GLint> TextureFile::format_map = { {0, 0}, {1, GL_RED}, {3, GL_RGB}, {4, GL_RGBA} };

void LDRTextureFile::LoadData()
{
	//通过stb_image导入图片文件
	data = stbi_load(path.c_str(), &width, &height, &component_num, 0); //倒数第二个参数为图片原始的通道数，不是输出的通道数
	//最后一个参数为需求的通道数（共rgba四通道），0代表返回图片的所有通道

	if (!data)
	{
		cout << "ERROR<LDRTextureFile> Failed to load data\n";
	}

	format = format_map[component_num];
	b_loaded = true;
}

void HDRTextureFile::LoadData()
{
	data = stbi_loadf(path.c_str(), &width, &height, &component_num, 0);
	if (!data)
	{
		cout << "ERROR<HDRTextureFile> Failed to load data\n";
	}
	format = format_map[component_num];
}
