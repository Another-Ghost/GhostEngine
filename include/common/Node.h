#pragma once
#include "Prerequisite.h"

class Node
{
public:

	Transform transform;

	virtual void Update(float dt);

	void SetParent(Node* parent_) { parent = parent_; }
	Node* GetParent() { return parent; }

	void AddChild(Node* child) { child_unit_set.emplace(child); }
	void RemoveChild(Node* child) { child_unit_set.erase(child); }

	Transform GetWorldTransform();
	void SetWorldTransform(Transform world_transform_);
	void SetLocalTransform(Transform local_transform_);

protected:
	Node* parent;

	set<Node*> child_unit_set;

};

