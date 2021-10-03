#include "TextureFile.h"
#define STB_IMAGE_IMPLEMENTATION    
#include <stb_image.h>

map<int, GLint> TextureFile::format_map = { {0, 0}, {1, GL_RED}, {3, GL_RGB}, {4, GL_RGBA} };

void LDRTextureFile::LoadData()
{
	//ͨ��stb_image����ͼƬ�ļ�
	data = stbi_load(path.c_str(), &width, &height, &component_num, 0); //�����ڶ�������ΪͼƬԭʼ��ͨ���������������ͨ����
	//���һ������Ϊ�����ͨ��������rgba��ͨ������0������ͼƬ������ͨ��

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
