#include "RenderUnit.h"
#include "RenderManager.h"
#include "RenderModule.h"
#include "AABBVolume.h"
#include "Mesh.h"

RenderUnit::RenderUnit(RenderModule* parent_, Mesh* mesh_, Material* material_): 
	mesh(mesh_), material(material_), b_rendered(true), parent(parent_)
{

}

void RenderUnit::Update(float dt)
{
	if (b_rendered == true)
	{
		RenderManager::GetSingleton().InsertRenderUnit(this);
	}
}

AABBVolume RenderUnit::GetAABBVolume()
{
	if (mesh)
	{
		if (parent)
		{
			vec3 scale = parent->GetWorldTransform().GetDimention();
			float max_value = scale.x > scale.y ? scale.x : scale.y;
			if (max_value < scale.z)
			{
				max_value = scale.z;
			}

			return mesh->aabb_volume * max_value;
		}
		else
		{
			return mesh->aabb_volume;
		}
	}
	return AABBVolume();
}

vec3 RenderUnit::GetPosition()
{
	/*Transform tf = parent->GetWorldTransform(); vec3 pos = parent->GetWorldTransform().GetPosition(); vec3 pos2 = tf.GetPosition();*/ 
	return parent->GetWorldTransform().GetPosition();
}

