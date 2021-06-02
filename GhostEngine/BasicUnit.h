#pragma once
#include "common/Unit.h"
class BasicUnit : public Unit
{
public:
	unique_ptr<RenderModule> render_module;

};

