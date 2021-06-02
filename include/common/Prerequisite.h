#pragma once
//This document must be included before other header file

#include "Config.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using std::cout;
using std::endl;
using std::vector;
using std::string;

using std::unique_ptr;
//using std::make_unique

using glm::vec2;
using glm::vec3;
using glm::mat4;
using glm::quat;
//const float PI = glm::pi<float>();

class Window;
class WindowManager;
class WindowFactory;
class Camera;

class SceneManager;
class ResourceManager;

class Unit;
class RenderUnit;
class RenderModule;

class Transform;

class Shader;
class Mesh;
class GeometryMesh;
class TriangleMesh;
class Texture;
class Material;
class PBRMaterial;
class PBRMaterialFactory;
enum class TextureType;
enum class MaterialType;
class Light;
class PointLight;