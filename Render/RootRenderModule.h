#pragma once
#include "RenderModule.h"
class RootRenderModule : public RenderModule
{
private:
	//屏蔽对父类GetParent调用
	virtual RenderModule* GetParent() { return RenderModule::parent; };
	virtual void SetParent(RenderModule* parent_) { RenderModule::parent = parent_; };
public:
	void SetParentNode(Node* parent_) { parent = parent_; }
	Node* GetParentNode() { return parent; }

	Transform GetWorldTransform() override;

	Node* parent;
	
};

