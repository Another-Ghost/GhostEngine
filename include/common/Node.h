#pragma once
#include "TransformableModule.h"
#include "Prerequisite.h"

class Node : public TransformableModule
{
public:

	//Transform local_transform;

	virtual void Update(float dt);

	void SetParent(Node* parent_) { parent = parent_; }
	Node* GetParent() { return parent; }

	void AddChild(Node* child) { child_node_set.emplace(child); }
	void RemoveChild(Node* child) { child_node_set.erase(child); }

	Transform GetWorldTransform();
	void SetWorldTransform(Transform world_transform_);
	void SetLocalTransform(Transform local_transform_);

protected:
	Node* parent;

	set<Node*> child_node_set;

};

