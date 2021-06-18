#pragma once
#include "MVPShader.h"

class HDRIShader : public MVPShader
{
public:
	HDRIShader(EquirectangularMap* hdr_map);

	void BindEquirectangulerMap(unsigned int tex_id);

	unsigned int RenderCubeMap(CubeMap* cube_map);

private:
	
};

