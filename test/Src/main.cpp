#include "Utils/Utils.hpp"

#include "EntropyApp.hpp"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
#include "Common/Transform.hpp"
#include "Eigen/Core"	
#include "Utils/Debug.hpp"
#include "Parser/ObjParser.hpp"
#include "Parser/FbxParser.hpp"
#include "Graphic/Material.hpp"
#include "Graphic/Texture.hpp"
#include "Geometry/Sphere.hpp"
#include "Light/PointLight.hpp"

using namespace Entropy;

std::shared_ptr<SceneNode> createRifle() {
	std::shared_ptr<SceneNode> rifleNode(new SceneNode());

	std::shared_ptr<StaticMeshComponent> rifleMeshComponent(new StaticMeshComponent());
	rifleMeshComponent->Initialize();
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
		rifleMeshComponent->GetMeshes().push_back(m);
	}
	auto rifleMat = std::make_shared<StandardPBRMaterial>();
	rifleMeshComponent->GetMaterials().push_back(rifleMat);

	rifleMat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Albedo.jpg", bimg::TextureFormat::RGB8))));
	rifleMat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Normal.jpg", bimg::TextureFormat::RGB8))));
	rifleMat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Metallic.jpg", bimg::TextureFormat::R8))));
	rifleMat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Roughness.jpg", bimg::TextureFormat::R8))));
	rifleMat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_AO.jpg", bimg::TextureFormat::R8))));

	rifleNode->GetTransform()->SetScale(Vector3f(0.015, 0.015, 0.015));
	rifleNode->AddComponent(rifleMeshComponent->SharedPtr());
	return rifleNode;
}



std::shared_ptr<SceneNode> createGun() {
	std::shared_ptr<SceneNode> rifleNode(new SceneNode());

	std::shared_ptr<StaticMeshComponent> rifleMeshComponent(new StaticMeshComponent());
	rifleMeshComponent->Initialize();
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
		rifleMeshComponent->GetMeshes().push_back(m);
	}
	auto rifleMat = std::make_shared<StandardPBRMaterial>();
	rifleMeshComponent->GetMaterials().push_back(rifleMat);

	rifleMat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Gun/gun_BC.png", bimg::TextureFormat::RGB8))));
	rifleMat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Gun/gun_N.png", bimg::TextureFormat::RGB8))));
	rifleMat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Gun/gun_M.png", bimg::TextureFormat::R8))));
	rifleMat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Gun/gun_N.png", bimg::TextureFormat::R8))));
	rifleMat->SetAmbientOcclusion(1.0);

	rifleNode->GetTransform()->SetScale(Vector3f(20, 20, 20));
	rifleNode->AddComponent(rifleMeshComponent->SharedPtr());

	rifleNode->GetTransform()->SetPosition(-1, 0, 0);

	return rifleNode;
}



std::shared_ptr<SceneNode> createSphere() {
	std::shared_ptr<SceneNode> sphereNode(new SceneNode());

	std::shared_ptr<StaticMeshComponent> sphereComponent(new StaticMeshComponent());
	sphereComponent->Initialize();

	// sphereComponent->GetMeshes().push_back((*ParseObj("Assets/bunny.obj"))[0]);
	auto mat = std::make_shared<StandardPBRMaterial>();
	sphereComponent->GetMaterials().push_back(mat);

	mat->SetAlbedo(ColorRGBA(128,128,128,255));
	mat->SetMetallic(0.9);
	mat->SetRoughness(0.4);
	mat->SetAmbientOcclusion(1.0);

	sphereNode->AddComponent(sphereComponent->SharedPtr());
	return sphereNode;
}


int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	// app.entropyCore->debugMode = true;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();

	// auto rifleNode = createRifle();
	auto rifleNode = createGun();
	scene->GetRootNode()->AddChild(rifleNode->SharedPtr());

	
	auto camera = std::shared_ptr<Camera>(std::make_shared<Camera>());
	auto cameraNode = std::make_shared<SceneNode>();
	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0,3,-5));
	cameraNode->GetTransform()->Rotate(0.3, Vector3f::UnitX());
	cameraNode->AddComponent(camera);
	scene->MainCamera = camera;
	scene->GetRootNode()->AddChild(cameraNode->SharedPtr());

	auto sphereNode = createSphere();

	scene->GetRootNode()->AddChild(sphereNode->SharedPtr());
	scene->SetOnTick([&](float deltaTime)
	{
		rifleNode->GetTransform()->Rotate(deltaTime *3.14 * 0.1,Vector3f::UnitY());
	});

	auto pointLightNode = std::make_shared<SceneNode>();
	auto pointLight = std::make_shared<PointLight>();
	pointLight->SetLightColor(Vector3f(1, 1, 1));
	pointLight->SetIntensive(3);
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