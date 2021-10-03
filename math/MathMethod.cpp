#include "MathMethod.h"
#include <iostream>


float MathMethod::MaxElement(const vec3& vector)
{
    float ans = vector.x > vector.y ? vector.x : vector.y;
    ans = ans > vector.z ? ans : vector.z;

    return ans;
}
