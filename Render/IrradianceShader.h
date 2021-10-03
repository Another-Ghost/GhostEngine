#pragma once
#include "MVPShader.h"
class IrradianceShader : public MVPShader
{
public:
	IrradianceShader();

	void RenderEnvIrradianceCubeMap(const CubeMap* irradiance_cubemap, unsigned int ori_cubemap_id);
};

