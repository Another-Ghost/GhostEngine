#include "BasicRenderer.h"
#include "RenderManager.h"
#include "BasicShader.h"
#include "BasicMaterial.h"
#include "RenderUnit.h"
#include "RenderModule.h"



BasicRenderer::BasicRenderer()
{
	basic_shader = new BasicShader();
}

void BasicRenderer::Update(float dt)
{
	for (const auto& pair : RenderManager::GetSingleton().basic_mat_unit_map)
	{
		basic_shader->BindMaterial(pair.first);

		for (const auto& render_unit : pair.second)
		{
			Mesh* mesh = render_unit->GetMesh();
			mat4 model = render_unit->GetParent()->GetWorldTransform().GetMatrix();
			basic_shader->SetModelMatrix(model);

			mesh->Draw();
		}
	}
}
