#include "PointLightFactory.h"
#include "PointLight.h"

Light* PointLightFactory::CreateLight() const
{
    return new PointLight();
}
