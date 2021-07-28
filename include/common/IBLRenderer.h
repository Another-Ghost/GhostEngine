#pragma once


#include "Renderer.h"
class IBLRenderer : public Renderer
{
public:
	IBLRenderer();

	void Update(float dt) override;


};

