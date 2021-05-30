#include "BasicRenderer.h"
#include "common/Shader.h"
#include "common/Transform.h"
#include "common/world.h"
#include "common/File.h"

// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphere_VAO = 0;
unsigned int index_count;
void RenderSphere()
{
	if (sphere_VAO == 0)
	{
		glGenVertexArrays(1, &sphere_VAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		index_count = indices.size();

		std::vector<float> data;
		for (std::size_t i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
		}
		glBindVertexArray(sphere_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	}

	glBindVertexArray(sphere_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, index_count, GL_UNSIGNED_INT, 0);
}

bool BasicRenderer::Initialize()
{
	camera = world->GetCamera();

	return true;
}

void BasicRenderer::BeginRender()
{
	glEnable(GL_DEPTH_TEST);

	basic_shader = new Shader("basic_vs", "basic_fs");

	//basic_scene = new Scene("backpack/backpack.obj");
	basic_shader->Use();
	basic_shader->SetInt("albedo_map", 0);
	basic_shader->SetInt("normal_map", 1);
	basic_shader->SetInt("metallic_map", 2);
	basic_shader->SetInt("roughness_map", 3);
	basic_shader->SetInt("ao_map", 4);

	albedo = Texture::LoadTexture("texture/pbr/rusted_iron/albedo.png");
	normal = Texture::LoadTexture("texture/pbr/rusted_iron/normal.png");
	metallic = Texture::LoadTexture("texture/pbr/rusted_iron/metallic.png");
	roughness = Texture::LoadTexture("texture/pbr/rusted_iron/roughness.png");
	ao = Texture::LoadTexture("texture/pbr/rusted_iron/ao.png");

	light_positions[0] = { vec3(0.f, 0.f, 15.f), };
	light_colors[0] = { vec3(300.f, 300.f, 300.f), };
	num_row = 1;
	num_column = 1;
	spacing = 2.5;

	basic_shader->Use();
	basic_shader->SetMat4("projection", camera->PerspectiveMatrix());

}

void BasicRenderer::Update(float dt)
{
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	basic_shader->Use();

	basic_shader->SetMat4("view", camera->ViewMatrix());
	basic_shader->SetVec3("cam_pos", camera->transform.GetPosition());
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metallic);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, roughness);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ao);


	for (int row = 0; row < num_row; ++row)
	{
		for (int col = 0; col < num_column; ++col)
		{
			mat4 model_matrix = mat4(1.f);
			model_matrix = glm::translate(model_matrix, vec3((col - num_column / 2.f) * spacing, (row - num_row / 2.f) * spacing, 0.f));
			basic_shader->SetMat4("model", model_matrix);

			RenderSphere();
		}
	}

}

