#pragma once
#include "BasicDependencies.h"

enum class TextureFileType
{
	LDR,
	HDR,
};

struct TextureFile
{
	TextureFile(TextureFileType type_, const string& path_) :
		path(path_), type(type_), b_loaded(false) {};

	string path;

	TextureFileType type;

	//void* data;

	int width;
	int height;
	int component_num;
	GLint format;

	bool b_loaded;

	virtual void LoadData() = 0;

	static map<int, GLint> format_map;
};

struct LDRTextureFile : TextureFile
{
	LDRTextureFile(const string& path_ = "", TextureFileType type_ = TextureFileType::LDR):
	TextureFile(type_, path_){}
	const unsigned char* data;
	void LoadData() override;
};

struct HDRTextureFile : TextureFile
{
	HDRTextureFile(const string& path_ = "", TextureFileType type_ = TextureFileType::HDR) :
		TextureFile(type_, path_) {}
	float* data;
	void LoadData() override;
};


