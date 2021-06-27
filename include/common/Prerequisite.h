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
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using glm::quat;
//const float PI = glm::pi<float>();

class Window;
class WindowManager;
class WindowFactory;
class WindowEventListener;

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
class PBRShader;
class SkyboxShader;

class Mesh;
class MeshFactory;
class GeometryMesh;
class TriangleMesh;
class CubeGeometryMesh;
class QuadGeometryMesh;

struct Texture;
struct Material;
struct PBRMaterial;
class PBRMaterialFactory;
enum class TextureType;
enum class MaterialType;
struct TextureFile;
enum class TextureFileType;
struct EquirectangularMap;

struct Light;
class LightFactory;
struct PointLight;

struct HDRTexture;
struct CubeMap;
class Renderer;
class IBLRenderer;

