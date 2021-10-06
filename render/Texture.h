#pragma once
#include "TextureFile.h"
#include <string>
#include <stb_image.h>

enum class TextureType
{
	EMPTY2D,

	SPECULAR,
	DIFFUSE,
	HEIGHT,

	ALBEDO,

	BASECOLOR, 
	NORMAL,
	ROUGHNESS,
	METALNESS,
	AO,
	METALNESSROUGHNESS,

	EQUIRECTANGULARMAP,
	CUBEMAP,

	ATTACHMENT,
};

struct Texture
{
	unsigned int id;	//? 改回id
	
	TextureType type; //类型，比如是漫反射贴图或者是镜面光贴图
	
	TextureFile* file;

	int width;
	int height;

	bool b_buffered;

	bool b_genarate_mipmap;

	GLint internal_format;

	GLenum data_format;

	GLenum data_type;

	GLint wrap_param;

	GLint min_filter_param;

	GLint mag_filter_param;

	Texture(TextureType type_, TextureFile* file_ = nullptr);
	
	virtual bool Buffer() = 0;	//? 看一下如何卸载缓存的材质

	static std::string TypeName(TextureType type)
	{
		switch (type)
		{
		case TextureType::DIFFUSE:
			return "diffuse";
		case TextureType::ALBEDO:
			return "albedo";
		case TextureType::SPECULAR:
			return "specular";
		case TextureType::BASECOLOR:
			return "basecolor";
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
			return string();
		}
	}

	//static unsigned int LoadTexture(const std::string& name); //? 在何处进行gamma校正
	/*debug*/
	void AddLabel(string _label) {
		glObjectLabel(GL_TEXTURE, id, -1, _label.c_str());
	}
};

