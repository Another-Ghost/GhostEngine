#pragma once
#include "MVPShader.h"
class PrefilterShader : public MVPShader
{
public:
	PrefilterShader();

	void RenderPrefilterCubeMap(const CubeMap* prefilter_cubemap, unsigned int ori_cubemap_id);

};

