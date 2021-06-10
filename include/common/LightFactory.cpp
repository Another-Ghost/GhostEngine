#include "LightFactory.h"
#include "Light.h"

Light* LightFactory::CreateLight() const
{
	return new Light();
}
