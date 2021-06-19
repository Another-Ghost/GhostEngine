#pragma once
#include "MVPShader.h"
class BRDFLUTShader : public MVPShader
{
public:
	BRDFLUTShader();

	void RenderBRDFLUT(const Texture* brdf_lut);
};

