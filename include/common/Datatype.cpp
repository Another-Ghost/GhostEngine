#include "Datatype.h"

glm::vec3 Datatype::ToGLMVec3(const aiVector3D& vec)
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec2 Datatype::ToGLMVec2(const aiVector2D& vec)
{
    return glm::vec2(vec.x, vec.y);
}
