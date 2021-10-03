#pragma once
#include "MVPShader.h"

class HDRIShader : public MVPShader
{
public:
	HDRIShader();

	void RenderCubeMap(const CubeMap* cube_map, PlaneTexture* hdr_tex);

private:
	
};

