#include "Utils/Utils.hpp"

#include "EntropyApp.hpp"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
#include "Common/Transform.hpp"
#include "Common/Camera.hpp"
#include "Eigen/Core"	
#include "Utils/Debug.hpp"
#include "Parser/ObjParser.hpp"
#include "Parser/FbxParser.hpp"
#include "Graphic/Material.hpp"
#include "Graphic/Texture.hpp"
#include "Graphic/SkyBox.hpp"
#include "Geometry/Sphere.hpp"
#include "Light/PointLight.hpp"

using namespace Entropy;


std::shared_ptr<SceneNode> createCerberus() {

	std::shared_ptr<SceneNode> objNode(new SceneNode("gunParent"));
	objNode->Initialize();
	std::shared_ptr<SceneNode> node(new SceneNode("gun"));
	node->Initialize();
	std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
	meshComponent->Initialize();
	std::string inputfile = "Assets/Cerberus_by_Andrew_Maximov/Cerberus_LP.fbx";

	std::vector<std::shared_ptr<Mesh>>* meshes;

	if (ends_with(inputfile, ".obj")) {
		meshes = ParseObj(inputfile);
	}
	else if (ends_with(inputfile, ".fbx")) {
		meshes = ParseFBX(inputfile);
	}
	else {
		return nullptr;
	}

	for (auto m : *meshes) {
		meshComponent->GetMeshes().push_back(m);
	}
	auto mat = std::make_shared<StandardPBRMaterial>();
	meshComponent->GetMaterials().push_back(mat);

	mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga", bimg::TextureFormat::RGB8))));
	mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga", bimg::TextureFormat::RGB8))));
	mat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga", bimg::TextureFormat::R8))));
	mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga", bimg::TextureFormat::R8))));
	mat->SetAmbientOcclusion(1);

	node->GetTransform()->SetScale(Vector3f(0.03, 0.03, 0.03));
	node->GetTransform()->SetPosition(Vector3f(0,2,0));
	node->GetTransform()->Rotate(3.14 /2, Vector3f::UnitZ());
	node->GetTransform()->Rotate(3.14 /2, Vector3f::UnitY());

	node->AddComponent(meshComponent->SharedPtr());

	objNode->AddChild(std::move(node));
	return objNode;
}


std::shared_ptr<SceneNode> createRifle() {
	std::shared_ptr<SceneNode> node(new SceneNode());
	node->Initialize();

	std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
	meshComponent->Initialize();
	std::string inputfile = "Assets/Rifle2/Rifle_2.fbx";

	std::vector<std::shared_ptr<Mesh>>* meshes;

	if (ends_with(inputfile, ".obj")) {
		meshes = ParseObj(inputfile);
	}
	else if (ends_with(inputfile, ".fbx")) {
		meshes = ParseFBX(inputfile);
	}
	else {
		return nullptr;
	}

	for (auto m : *meshes) {
		meshComponent->GetMeshes().push_back(m);
	}
	auto mat = std::make_shared<StandardPBRMaterial>();
	meshComponent->GetMaterials().push_back(mat);

	mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Albedo.jpg", bimg::TextureFormat::RGB8))));
	mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Normal.jpg", bimg::TextureFormat::RGB8))));
	mat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Metallic.jpg", bimg::TextureFormat::R8))));
	mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Roughness.jpg", bimg::TextureFormat::R8))));
	mat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_AO.jpg", bimg::TextureFormat::R8))));

	node->GetTransform()->SetScale(Vector3f(0.015, 0.015, 0.015));
	node->AddComponent(meshComponent->SharedPtr());
	return node;
}



std::shared_ptr<SceneNode> createGun() {
	std::shared_ptr<SceneNode> node(new SceneNode());
	node->Initialize();

	std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
	meshComponent->Initialize();
	std::string inputfile = "Assets/Gun/gun.obj";

	std::vector<std::shared_ptr<Mesh>>* meshes;

	if (ends_with(inputfile, ".obj")) {
		meshes = ParseObj(inputfile);
	}
	else if (ends_with(inputfile, ".fbx")) {
		meshes = ParseFBX(inputfile);
	}
	else {
		return nullptr;
	}

	for (auto m : *meshes) {
		meshComponent->GetMeshes().push_back(m);
	}
	auto mat = std::make_shared<StandardPBRMaterial>();
	meshComponent->GetMaterials().push_back(mat);

	mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Gun/gun_BC.png", bimg::TextureFormat::RGB8))));
	mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Gun/gun_N.png", bimg::TextureFormat::RGB8))));
	mat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Gun/gun_M.png", bimg::TextureFormat::R8))));
	mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Gun/gun_N.png", bimg::TextureFormat::R8))));
	mat->SetAmbientOcclusion(1.0);

	node->GetTransform()->SetScale(Vector3f(20, 20, 20));
	node->AddComponent(meshComponent->SharedPtr());

	node->GetTransform()->SetPosition(-1, 0, 0);

	return node;
}


int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	// app.entropyCore->debugMode = true;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();


	auto skybox = std::make_shared<SkyBox>();
	skybox->SetHdrTexture(std::shared_ptr<Texture>(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/newport_loft.hdr", bimg::TextureFormat::RGB8)))));
	scene->SetSkybox(skybox);

	// auto gunNode = createCerberus();
	auto gunNode = createGun();
	scene->GetRootNode()->AddChild(gunNode->SharedPtr());

	
	auto camera = std::shared_ptr<Camera>(std::make_shared<Camera>());
	auto cameraNode = std::make_shared<SceneNode>();
	cameraNode->Initialize();

	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0,3,-5));
	cameraNode->GetTransform()->Rotate(0.3, Vector3f::UnitX());
	cameraNode->AddComponent(camera);
	scene->MainCamera = camera;
	scene->GetRootNode()->AddChild(cameraNode->SharedPtr());


	scene->SetOnTick([&](float deltaTime)
	{
		gunNode->GetTransform()->Rotate(deltaTime *3.14 * 0.1,Vector3f::UnitY());
		cameraNode->GetTransform()->Rotate(deltaTime *3.14 * 0.1,Vector3f::UnitY());
		camera->updateViewMatrix(true);
	});

	auto pointLightNode = std::make_shared<SceneNode>();
	pointLightNode->Initialize();
	auto pointLight = std::make_shared<PointLight>();
	pointLight->SetLightColor(Vector3f(1, 1, 1));
	pointLight->SetIntensive(10);
	pointLight->Initialize();
	pointLightNode->AddComponent(pointLight);
	pointLightNode->GetTransform()->SetPosition(2, 2, 0);

	std::shared_ptr<StaticMeshComponent> sphereComponent(new StaticMeshComponent());
	sphereComponent->Initialize();
	sphereComponent->GetMeshes().push_back(GenerateSphere(0.1));
	sphereComponent->SetEnableLighting(false);
	auto mat = std::make_shared<StandardPBRMaterial>();
	sphereComponent->GetMaterials().push_back(mat);

	mat->SetAlbedo(ColorRGBA(128, 128, 128, 255));
	mat->SetMetallic(0.9);
	mat->SetRoughness(0.4);
	mat->SetAmbientOcclusion(1.0);
	auto lightColor = pointLight->GetLightColor();
	mat->SetEmissive(ColorRGBA(lightColor.x()*255, lightColor.y()*255, lightColor.z() * 255,  std::min(int( pointLight->GetInstensive() * 255),255)));
	pointLightNode->AddComponent(sphereComponent);
	

	try {
		return app.run(_argc, _argv);

	}catch(std::exception e) {
		Log(e.what());
	}
}