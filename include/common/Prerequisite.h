#pragma once
//This document must be included before other header file

#include "Config.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Transform.h"
#include "File.h"
#include "Debug.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::map;
using std::set;
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
class CameraFactory;

class SceneManager;
class ResourceManager;

class Unit;
class RenderUnit;
class RenderModule;
class RootRenderModule;
class RenderManager;

struct Transform;

class Shader;
class Mesh;
class MeshFactory;
class GeometryMesh;
class TriangleMesh;
struct Texture;
struct Material;
struct PBRMaterial;
class PBRMaterialFactory;
enum class TextureType;
enum class MaterialType;
struct Light;
struct PointLight;