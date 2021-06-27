#pragma once
#include "MVPShader.h"

class HDRIShader : public MVPShader
{
public:
	HDRIShader();

	void BindEquirectangulerMap(unsigned int tex_id);

	void RenderCubeMap(const CubeMap* cube_map, unsigned int hdr_tex_id);

private:
	
};

