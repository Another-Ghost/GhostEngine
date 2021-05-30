#pragma once
#include <string>


enum class TextureType
{
	DIFFUSE,
	SPECULAR,
	NORMAL,
	HEIGHT,

};


struct Texture
{
	unsigned int id;
	TextureType type; //类型，比如是漫反射贴图或者是镜面光贴图
	std::string path; //通过不同路径来比较不同纹理（将所有加载过的纹理全局储存，每当我们想加载一个纹理的时候，首先去检查它有没有被加载过）

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
		case TextureType::HEIGHT:
			return "height";
		default:
			return nullptr;
		}
	}

	static unsigned int LoadTexture(const std::string& name);

	static unsigned int LoadTexture(const std::string& name, const std::string& directory);	//? 在何处进行gamma校正


};

