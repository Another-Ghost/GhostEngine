#include "CameraFactory.h"

Camera* CameraFactory::CreateCamera() const 
{
    return new Camera();
}
