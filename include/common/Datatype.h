#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Datatype
{
public:
	static glm::vec3 ToGLVec3(const aiVector3D& vec);

	static glm::vec2 ToGLVec2(const aiVector2D& vec);
};

