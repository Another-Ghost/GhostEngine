#pragma once
#include "TransformableModule.h"	//?
#include "RenderUnit.h"


enum class RenderModuleType
{
	DEFAULT,
	ROOT,
};

class RenderModule : public TransformableModule
{
protected:
	RenderModule* parent;

	bool b_rendered;

public:

	//Transform local_transform;

	//vector<RenderUnit*> render_unit_array;
	RenderUnit* render_unit;

	vector<RenderModule*> child_array;

	RenderModule() : b_rendered(true) {}

	virtual void Update(float dt) override;

	virtual RenderModule* GetParent() { return parent; }
	virtual void SetParent(RenderModule* parent_) { parent = parent_; }

	void AddChild(RenderModule* child); 

	//void AddRenderUnit(RenderUnit* ru);
	void AttachRenderUnit(RenderUnit* ru) { render_unit = ru; }

	virtual Transform GetWorldTransform() override;

};

