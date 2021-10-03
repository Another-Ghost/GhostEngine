#include "Datatype.h"

glm::vec3 Datatype::ToGLVec3(const aiVector3D& vec)
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec2 Datatype::ToGLVec2(const aiVector2D& vec)
{
    return glm::vec2(vec.x, vec.y);
}
