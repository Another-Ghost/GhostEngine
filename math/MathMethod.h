#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;

class MathMethod
{
public:
	template<typename T>
	static T Lerp(T a, T b, T f) 
	{
		return a + f * (b - a);
	}

	static float MaxElement(const vec3& vector);
};

