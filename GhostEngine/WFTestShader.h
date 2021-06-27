#pragma once
#include "common\PBRShader.h"

class WFTestShader : public PBRShader
{
public:
	WFTestShader();

	void SetRoughness(float roughness_);

	void BindSSDirectionalAlbedoLUT(unsigned int tex_id);

private:
	float roughness;
};

