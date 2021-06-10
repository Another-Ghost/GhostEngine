#pragma once
#include "Prerequisite.h"

struct CubeMap
{
	CubeMap(int width_, int height_);

	unsigned int texture_id;

	HDRTexture* equirectangular_texture;

	int width;
	int height;

	void Buffer();
};

