#pragma once
#include "MVPShader.h"
class SkyboxShader : public MVPShader
{
public:
    SkyboxShader();

    void RenderSkybox(unsigned int skybox_tex_id);

};

