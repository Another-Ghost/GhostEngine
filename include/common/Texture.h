#pragma once
#include "Prerequisite.h"
#include <string>
#include <stb_image.h>

enum class TextureType
{
	NONE,
	DIFFUSE,
	SPECULAR,
	NORMAL,
	ROUGHNESS,
	METALNESS,
	AO,
	HEIGHT,
};

struct Texture
{
	unsigned int id;
	
	unsigned char* data;
	TextureType type; //类型，比如是漫反射贴图或者是镜面光贴图
	std::string path; //通过不同路径来比较不同纹理（将所有加载过的纹理全局储存，每当我们想加载一个纹理的时候，首先去检查它有没有被加载过）

	int width;
	int height;
	int component_num;


	Texture(const string& path, TextureType type_);

	static std::string TypeName(TextureType type)
	{
		switch (type)
		{
		case TextureType::DIFFUSE:
			return "diffuse";
		case TextureType::SPECULAR:
			return "specular";
		case TextureType::NORMAL:
			return "normal";
		case TextureType::METALNESS:
			return "metalness";
		case TextureType::AO:
			return "ao";
		case TextureType::HEIGHT:
			return "height";
		case TextureType::ROUGHNESS:
			return "roughness";
		default:
			return nullptr;
		}
	}

	void Buffer();	//? 看一下如何卸载缓存的材质

	bool b_buffered;

	static unsigned int LoadTexture(const std::string& name);

	static unsigned int LoadTexture(const std::string& name, const std::string& directory);	//? 在何处进行gamma校正

};

