#pragma once
//This document must be included before other header file

#include "Config.h"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <queue>
#include <algorithm>


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
using std::multimap;
using std::multiset;
using std::unique_ptr;
using std::shared_ptr;
using std::priority_queue;
using std::make_unique;
using std::make_shared;

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

class Node;
class RenderNode;
class RenderModule;
class RenderUnit;
class RootRenderModule;
class RenderManager;

struct Transform;

class Shader;
class MVPShader;
class BasicShader;
class PBRShader;
class SkyboxShader;
class ChannelCombinationShader;
class PBRLightingPassShader;
class SSAOShader;
class SSAOBlurShader;
class OutputShader;

class Mesh;
class MeshFactory;
class GeometryMesh;
class TriangleMesh;
class CubeGeometryMesh;
class QuadGeometryMesh;
struct ExpandedVertex;

struct Texture;
struct PlaneTexture;
struct Material;
struct BasicMaterial;
struct PBRMaterial;
class MaterialFactory;
class PBRMaterialFactory;
enum class MaterialType;
enum class TextureType;
struct TextureFile;
struct HDRTextureFile;
enum class TextureFileType;
struct EquirectangularMap;
struct AttachmentTexture;


struct Light;
class LightFactory;
struct PointLight;

struct HDRTexture;
struct CubeMap;
class Renderer;
class IBLRenderer;
class BasicRenderer;
class PostProcessRenderer;
class PBRDeferRenderer;

struct Frustum;
struct Plane;
struct AABBVolume;

struct GBuffer;