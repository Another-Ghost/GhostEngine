#pragma once
#include "Prerequisite.h"

class Unit
{
public:

	Transform transform;





	virtual void Update(float dt);

	void SetParent(Unit* parent_) { parent = parent_; }
	Unit* GetParent() { return parent; }

	void AddChild(Unit* child) { child_unit_array.emplace(child); }
	void RemoveChild(Unit* child) { child_unit_array.erase(child); }

	Transform GetWorldTransform();
	void SetWorldTransform(Transform world_transform_);
	void SetLocalTransform(Transform local_transform_);

protected:
	Unit* parent;

	set<Unit*> child_unit_array;

};

