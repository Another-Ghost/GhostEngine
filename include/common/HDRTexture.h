#pragma once
#include "Texture.h"
struct HDRTexture : Texture	//change name to EquirectangularTexture
{
	HDRTexture(const string& path_, TextureType type_) : Texture(path_, type_){}

	float* data;
	void LoadData() override;
	void Buffer() override;

};

