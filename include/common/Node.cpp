#include "Node.h"

void Node::Update(float dt)
{
	//transform = local_transform * parent->transform;
	for (const auto& child : child_unit_set)
	{
		child->Update(dt);
	}
}

Transform Node::GetWorldTransform()
{
	//if(parent)
	//{
	//	return local_transform * parent->GetWorldTransform();
	//}
	//else
	//{
	//	return local_transform;
	//}
	return transform;
}

void Node::SetWorldTransform(Transform world_transform_)
{
	//world_transform = world_transform_;
	//if (parent)
	//{
	//	local_transform = Transform(world_transform.GetMatrix() * glm::inverse(parent->local_transform.GetMatrix()));
	//	for (const auto& child : child_unit_array)
	//	{

	//	}
	//}
	//else
	//{
	//	local_transform = world_transform;
	//}
}

void Node::SetLocalTransform(Transform local_transform_)
{
	//local_transform = local_transform_;
	//if (parent)
	//{
	//	world_transform = Transform()
	//}
	//else
	//{
	//	local_transform = world_transform;
	//}
}
