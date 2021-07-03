#pragma once
#include "Module.h"

enum class RenderModuleType
{
	DEFAULT,
	ROOT,
};

class RenderModule : public Module
{
protected:
	RenderModule* parent;

	bool b_rendered;

public:
	Transform transform;

	vector<RenderUnit*> render_unit_array;

	RenderModule() : b_rendered(true) {}

	virtual void Update(float dt) override;

	virtual RenderModule* GetParent() { return parent; }
	virtual void SetParent(RenderModule* parent_) { parent = parent_; }


};

