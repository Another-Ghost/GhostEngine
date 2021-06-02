#pragma once
#include "common/RenderModule.h"

class BasicRenderModule : public RenderModule
{
	GeometryMesh* mesh;
	PBRMaterial* material;
};

