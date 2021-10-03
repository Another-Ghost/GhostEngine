#pragma once
//This document must be included before other header file
#include "Config.h"
#include "BasicDependencies.h"
#include "MathMethod.h"

#include "Transform.h"
#include "File.h"
#include "Debug.h"


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
class PrefilterShader;
class IrradianceShader;

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
class AABBModule;

struct GBuffer;

class LightProbe;
class ReflectionProbe;
class LightProbeRenderer;

class ShadowRenderer;
class PointShadowShader;
class PointShadowDepthShader;

class FrameBuffer;