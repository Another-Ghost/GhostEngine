#pragma once
#include "RenderModule.h"
class RootRenderModule : public RenderModule
{
private:
	//屏蔽对父类GetParent调用
	virtual RenderModule* GetParent() { return RenderModule::parent; };
	virtual void SetParent(RenderModule* parent_) { RenderModule::parent = parent_; };
public:
	void SetParentUnit(Unit* parent_) { parent = parent_; }
	Unit* GetParentUnit() { return parent; }

	Unit* parent;
	
};

