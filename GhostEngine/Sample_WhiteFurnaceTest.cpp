#include "common/Root.h"
#include "common/WindowManager.h"
//#include "BasicWindowFactory.h"
//#include "BasicWindow.h"
//#include "BasicCamera.h"
//#include "BasicNode.h"
#include "common/RenderNode.h"
#include "common/RenderModule.h"
#include "common/ResourceManager.h"
#include "common/PBRMaterial.h"
#include "common/File.h"
#include "common/PointLight.h"
#include "common/RootRenderModule.h"
#include "common/SphereGeometryMesh.h"
#include "common/SceneManager.h"
#include "common/CameraFactory.h"
#include "common/SphereGeometryMeshFactory.h"
#include "common/CubeGeometryMeshFactory.h"
#include "common/PointLightFactory.h"
#include "common/TextureFile.h"
#include "common/RenderUnit.h"
//#include "common/Model.h"
#include "common/PBRRenderer.h"
#include "common/RenderManager.h"
#include "common/AABBModule.h"
#include "common/ReflectionProbe.h"
#include "WFTestRenderer.h"

#include <memory>

#define SAMPLE_WFTEST
#ifdef SAMPLE_WFTEST

int main()
{
	//Init
	Root* root = new Root(1280, 720, "PBR_Demo");
	//Root* root = new Root(1920, 1080, "PBR Demo");
	//BasicWindowFactory* window_factory = new BasicWindowFactory();
	//BasicWindow* window = dynamic_cast<BasicWindow*>(WindowManager::GetSingleton().CreateWindow(window_factory, 1280, 720, "Demo"));

	HDRTextureFile* hdr_file = dynamic_cast<HDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::HDR, true, File::GetTexturePath("hdr/night.hdr")));
	RenderManager::GetSingleton().BindSkyboxTexture(hdr_file);	//? 改成绑定到SceneManager中的skybox class 对象

	//WFTestRenderer* renderer = new WFTestRenderer();
	PBRRenderer* renderer = new PBRRenderer();
	Root::GetSingleton().Initialize(renderer);

	Camera* camera = SceneManager::GetSingleton().CreateCamera(CameraFactory());
	camera->SetPosition(vec3(0, 4, 6));
	//camera->SetYaw(45.f);

	Window* window = WindowManager::GetSingleton().s_current_window;
	//window->SetCamera(camera);	//? 改成在内部initialize里从SceneManger获取
	window->AddEventListener(camera);

	//window->AddEventListener(renderer);



/*Material*/
	PBRMaterial* material = dynamic_cast<PBRMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::PBR));
	LDRTextureFile* basecolor_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/gold/albedo.png")));	//rusted_iron, painted_metal, gold
	material->basecolor_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::BASECOLOR, true, basecolor_file);
	//material->albedo_map->Buffer();
	LDRTextureFile* normal_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/gold/normal.png")));
	material->normal_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::NORMAL, true, normal_file);
	//material->normal_map->Buffer();
	LDRTextureFile* metalness_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/gold/metallic.png")));
	material->metalness_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::METALNESS, true, metalness_file);
	//material->metalness_map->Buffer();
	LDRTextureFile* roughness_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/gold/roughness.png")));
	material->roughness_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ROUGHNESS, true, roughness_file);
	//material->roughness_map->Buffer();
	LDRTextureFile* ao_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/gold/ao.png")));
	material->ao_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::AO, true, ao_file);
	//material->ao_map->Buffer();


	PBRMaterial* cube_mat = dynamic_cast<PBRMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::PBR));
	LDRTextureFile* tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/paint_green/basecolor.png")));	//rusted_iron, painted_metal, gold
	cube_mat->basecolor_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::BASECOLOR, true, tex_file);
	tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/paint_green/normal.png")));
	cube_mat->normal_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::NORMAL, true, tex_file);
	tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/paint_green/metalness.png")));
	cube_mat->metalness_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::METALNESS, true, tex_file);
	tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/paint_green/roughness.png")));
	cube_mat->roughness_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ROUGHNESS, true, tex_file);
	tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/paint_green/ao.png")));
	cube_mat->ao_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::AO, true, tex_file);

	PBRMaterial* point_mat = dynamic_cast<PBRMaterial*>(ResourceManager::GetSingleton().CreateMaterial(MaterialType::PBR));
	LDRTextureFile* point_tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/plastic/albedo.png")));	//rusted_iron, painted_metal, gold
	point_mat->basecolor_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::BASECOLOR, true, point_tex_file);
	point_tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/plastic/normal.png")));
	point_mat->normal_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::NORMAL, true, point_tex_file);
	point_tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/plastic/metallic.png")));
	point_mat->metalness_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::METALNESS, true, point_tex_file);
	point_tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/plastic/roughness.png")));
	point_mat->roughness_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::ROUGHNESS, true, point_tex_file);
	point_tex_file = dynamic_cast<LDRTextureFile*>(ResourceManager::GetSingleton().CreateTextureFile(TextureFileType::LDR, true, File::GetTexturePath("pbr/plastic/ao.png")));
	point_mat->ao_map = ResourceManager::GetSingleton().CreatePlaneTexture(TextureType::AO, true, point_tex_file);



	/*Mesh*/
	SphereGeometryMesh* sphere_mesh = dynamic_cast<SphereGeometryMesh*>(ResourceManager::GetSingleton().CreateMesh(SphereGeometryMeshFactory())); //? should be created by the resource manager
	CubeGeometryMesh* cube_mesh = dynamic_cast<CubeGeometryMesh*>(ResourceManager::GetSingleton().CreateMesh(CubeGeometryMeshFactory()));

	/*Node*/

		//RenderNode* sphere_node = new RenderNode();
		//RootRenderModule* sphere_root_render_module = new RootRenderModule();
		//ResourceManager::GetSingleton().CreateRenderUnit(sphere_root_render_module, sphere_mesh, material);
		//sphere_node->AttachRenderModule(sphere_root_render_module);
		//sphere_node->local_transform.SetDimension({ 2.f, 2.f, 2.f });
		//sphere_node->local_transform.SetPosition({ 2.f, 4.f, 2.f });

		//int column_num = 7;
		//for (int col = 0; col < column_num; ++col) {
	/*bottom*/
	RenderNode* cube_node = new RenderNode();
	RootRenderModule* root_render_module = new RootRenderModule();
	root_render_module->local_transform.SetPosition({ 0.f, 0.f, 0.f });
	//root_render_module->local_transform.SetDimension({ 20.f, 1.f, 20.f });
	root_render_module->local_transform.SetDimension({ 8.f, 1.f, 8.f });
	ResourceManager::GetSingleton().CreateRenderUnit(root_render_module, cube_mesh, cube_mat);
	cube_node->AttachRenderModule(root_render_module);

	//	sphere_unit->transform.SetPosition({ (col - column_num / 2.f) * 2.5f, 0.f, 0.f });
	//}

	//back
	RenderNode* wall1 = new RenderNode();
	RootRenderModule* wall1_root_rm = new RootRenderModule();
	ResourceManager::GetSingleton().CreateRenderUnit(wall1_root_rm, cube_mesh, cube_mat);
	wall1->AttachRenderModule(wall1_root_rm);
	wall1->local_transform.SetDimension({ 8.f, 8.f, 0.5f });
	wall1->local_transform.SetPosition({ 0.f, 4.f, -8.f });
	wall1->local_transform.SetYaw(0.f);

	//right
	RenderNode* wall2 = new RenderNode();
	RootRenderModule* wall2_root_rm = new RootRenderModule();
	ResourceManager::GetSingleton().CreateRenderUnit(wall2_root_rm, cube_mesh, material);
	wall2->AttachRenderModule(wall2_root_rm);
	wall2->local_transform.SetDimension({ 8.f, 8.f, 0.5f });
	wall2->local_transform.SetPosition({ 8.f, 4.f, 0.f });
	wall2->local_transform.SetYaw(90.f);

	//left
	RenderNode* wall3 = new RenderNode();
	RootRenderModule* wall3_root_rm = new RootRenderModule();
	ResourceManager::GetSingleton().CreateRenderUnit(wall3_root_rm, cube_mesh, cube_mat);
	wall3->AttachRenderModule(wall3_root_rm);
	wall3->local_transform.SetDimension({ 8.f, 8.f, 0.5f });
	wall3->local_transform.SetPosition({ -8.f, 2.f, 0.f });
	wall3->local_transform.SetYaw(-90.f);

	//front
	RenderNode* wall4 = new RenderNode();
	RootRenderModule* wall4_root_rm = new RootRenderModule();
	ResourceManager::GetSingleton().CreateRenderUnit(wall4_root_rm, cube_mesh, cube_mat);
	wall4->AttachRenderModule(wall4_root_rm);
	wall4->local_transform.SetDimension({ 8.f, 8.f, 0.5f });
	wall4->local_transform.SetPosition({ 0.f, 2.f, 8.f });
	wall4->local_transform.SetYaw(180.f);

	//top
	RenderNode* wall6 = new RenderNode();
	RootRenderModule* wall6_root_rm = new RootRenderModule();
	ResourceManager::GetSingleton().CreateRenderUnit(wall6_root_rm, cube_mesh, cube_mat);
	wall6->AttachRenderModule(wall6_root_rm);
	wall6->local_transform.SetDimension({ 8.f, 0.5f, 8.f });
	wall6->local_transform.SetPosition({ 0.f, 8.f, 0.f });

	//middle
	RenderNode* wall5 = new RenderNode();
	RootRenderModule* wall5_root_rm = new RootRenderModule();
	ResourceManager::GetSingleton().CreateRenderUnit(wall5_root_rm, cube_mesh, point_mat);
	wall5->AttachRenderModule(wall5_root_rm);
	wall5->local_transform.SetDimension({ 0.8f, 0.8f, 0.8f });
	wall5->local_transform.SetPosition({ -3.f, 4.f, 0.f });



	//origin
	//RenderNode* box = new RenderNode();
	//RootRenderModule* box_root_rm = new RootRenderModule();
	//ResourceManager::GetSingleton().CreateRenderUnit(box_root_rm, cube_mesh, cube_mat);
	//box->AttachRenderModule(box_root_rm);
	//box->local_transform.SetDimension({ 1.f, 1.f, 1.f });
	//box->local_transform.SetPosition({ 0.f, 0.f, 0.f });
	//box->local_transform.SetYaw(-90.f);

	//Triangle helmet
	//RenderNode* tri_node = new RenderNode();
	//tri_node->AttachRenderModule(ResourceManager::GetSingleton().gltf_loader->LoadFile(File::GetModelPath("DamagedHelmet/DamagedHelmet.gltf")));	//DamagedHelmet/DamagedHelmet.gltf, glTF-pbrSpecularGlossiness/BoomBox.gltf, WaterBottle/WaterBottle.gltf
	//tri_node->local_transform.SetPosition({ 0, 8, 0 });
	//tri_node->local_transform.SetOrientation(glm::angleAxis(glm::radians(90.f), vec3(1.f, 0.f, 0.f)));
	////tri_node->local_transform.SetOrientation(glm::angleAxis(glm::radians(90.f), vec3(0.f, 1.f, 0.f)));
	////tri_node->local_transform.SetDimension({ 1, 1, 1 });
	//tri_node->local_transform.SetDimension({ 2, 2, 2 });


	//RenderNode* tri_node2 = new RenderNode();
	//tri_node2->AttachRenderModule(ResourceManager::GetSingleton().gltf_loader->LoadFile(File::GetModelPath("WaterBottle/WaterBottle.gltf")));	//DamagedHelmet/DamagedHelmet.gltf, glTF-pbrSpecularGlossiness/BoomBox.gltf, WaterBottle/WaterBottle.gltf
	//tri_node2->local_transform.SetPosition({ 5, 5, 0 });
	////tri_node->local_transform.SetOrientation(glm::angleAxis(glm::radians(90.f), vec3(1.f, 0.f, 0.f)));
	//tri_node2->local_transform.SetOrientation(glm::angleAxis(glm::radians(90.f), vec3(0.f, 1.f, 0.f)));
	////tri_node->local_transform.SetDimension({ 1, 1, 1 });
	//tri_node2->local_transform.SetDimension({ 20, 20, 20 });


/*Light*/
	//PointLight* light = dynamic_cast<PointLight*>(SceneManager::GetSingleton().CreateLight(PointLightFactory()));
	//light->postion = vec3(0.f, 4.f, 0.f);
	//light->color = vec3(1.f, 0.f, 1.f);
	//light->intensity = 300.f;

	PointLight* light = dynamic_cast<PointLight*>(SceneManager::GetSingleton().CreateLight(PointLightFactory()));
	light->postion = vec3(0.f, 5.f, 0.f);
	light->intensity = 200.f;

	//light = dynamic_cast<PointLight*>(SceneManager::GetSingleton().CreateLight(PointLightFactory()));
	//light->postion = vec3(0.f, 4.f, 0.f);
	//light->color = vec3(1.f, 0.f, 1.f);
	//light->intensity = 150.f;

	//tri_node->AttachRenderModule(model.LoadScene(File::GetModelPath("BoomBoxSpecularGlossiness/BoomBox-Default.gltf"), MaterialType::PBR));	//backpack/backpack.obj, nanosuit/nanosuit.obj BoomBoxSpecularGlossiness/BoomBox-Default.gltf
	//tri_node->local_transform.SetPosition({ 0, 5, 0 });
	//tri_node->local_transform.SetDimension({ 10, 10, 10 });

	/*Light Probe*/

	ReflectionProbe* probe1 = SceneManager::GetSingletonPtr()->CreateReflectionProbe({ 0, 4, 0 }, AABBModule(AABBVolume({ 8, 4, 8 }), { 0, 4, 0 }));
	//SceneManager::GetSingletonPtr()->CreateReflectionProbe({ 4, 8, -4 }, AABBModule());


	//RenderNode* point1 = new RenderNode();
	//RootRenderModule* point1_root_rm = new RootRenderModule();
	//ResourceManager::GetSingleton().CreateRenderUnit(point1_root_rm, sphere_mesh, point_mat);
	//point1->AttachRenderModule(point1_root_rm);
	//point1->local_transform.SetDimension({ 0.5f, 0.5f, 0.5f });
	//shared_ptr<AABBModule> aabb = probe1->aabb_module;
	//point1->local_transform.SetPosition(aabb->volume.GetCorner(aabb->GetWorldPosition(), { 1.f, 1.f, 1.f }));

	//RenderNode* point2 = new RenderNode();
	//RootRenderModule* point2_root_rm = new RootRenderModule();
	//ResourceManager::GetSingleton().CreateRenderUnit(point2_root_rm, sphere_mesh, point_mat);
	//point2->AttachRenderModule(point2_root_rm);

	//point2->local_transform.SetDimension({ 1.0f, 1.f, 1.f });
	//point2->local_transform.SetPosition({ -4, 3, 4 });

	//point2->local_transform.SetDimension({ 0.5f, 0.5f, 0.5f });
	//point2->local_transform.SetPosition(aabb->volume.GetCorner(aabb->GetWorldPosition(), { -1.f, -1.f, -1.f }));



/*Loop*/
	float max_delta_time = 1.f / 60.f; 
	float current_frame_time;
	float delta_time = 1 / 60.f;
	float last_frame_time = 0.f;
	while (!window->WindowShouldClose())
	{
		root->Update(delta_time);

		current_frame_time = window->GetCurrentFrameTime();

		delta_time = std::min(current_frame_time - last_frame_time, max_delta_time);
		last_frame_time = current_frame_time;
	}
	Root::GetSingleton().Terminate();

	return 0;
}

#endif