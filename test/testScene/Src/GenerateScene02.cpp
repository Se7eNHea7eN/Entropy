#include "GenerateScene.hpp"
#include "Utils/Utils.hpp"

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
#include "Geometry/Sphere.hpp"

using namespace Entropy;
#define PI 3.14159265359

void GenerateScene02(Entropy::Scene* scene) {
	// auto skybox = std::make_shared<SkyBox>();
	// skybox->SetHdrTexture(std::shared_ptr<Texture>(
	// 	new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/prefilter.dds", bimg::TextureFormat::RGB8)))));
	// scene->SetSkybox(skybox);

	auto camera = std::shared_ptr<Camera>(std::make_shared<Camera>());
	camera->SetFovY(PI / 8);
	auto cameraNode = std::make_shared<SceneNode>("Camera");
	cameraNode->Initialize();

	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0, 0, -10));
	// cameraNode->GetTransform()->Rotate(0.6, Vector3f::UnitX());
	cameraNode->AddComponent(camera);
	scene->MainCamera = camera;
	scene->GetRootNode()->AddChild(cameraNode->SharedPtr());


	auto sphereMesh = GenerateSphere(0.3f,128, 128);
	sphereMesh->CalculateTangents();

	for (int i = 0; i < 8; i++) {
		std::shared_ptr<SceneNode> node(new SceneNode("Sphere"));
		node->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(ColorRGBA(128, 128, 128, 255));

		// mat->SetMetallic(1 - i/8.f);
		mat->SetMetallic(1);
		mat->SetRoughness(i / 8.f);
		mat->SetAmbientOcclusion(1);
		mat->SetIBL(
			new Texture2D(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/irradiance.dds", bimg::TextureFormat::RGB8))),
			new Texture2D(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/prefilter.dds", bimg::TextureFormat::RGB8))),
			new Texture2D(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/brdf.png", bimg::TextureFormat::RGBA8)))
		);
		node->GetTransform()->SetPosition(Vector3f(i-4, 0, 0));
		node->AddComponent(meshComponent->SharedPtr());

		scene->GetRootNode()->AddChild(node->SharedPtr());
	}

	
	auto pointLightNode = std::make_shared<SceneNode>("PointLight");
	scene->GetRootNode()->AddChild(pointLightNode->SharedPtr());
	pointLightNode->Initialize();
	auto pointLight = std::make_shared<PointLight>();
	pointLight->SetLightColor(Vector3f(1, 1, 1));
	pointLight->SetIntensive(10);
	pointLight->Initialize();
	pointLightNode->AddComponent(pointLight);
	pointLightNode->GetTransform()->SetPosition(2, 1, 0);
	
	std::shared_ptr<StaticMeshComponent> sphereComponent(new StaticMeshComponent());
	sphereComponent->Initialize();
	sphereComponent->GetMeshes().push_back(GenerateSphere(0.1));
	sphereComponent->SetEnableLighting(false);
	auto mat = std::make_shared<StandardPBRMaterial>();
	sphereComponent->GetMaterials().push_back(mat);
	pointLightNode->AddComponent(sphereComponent);
	
	mat->SetAlbedo(ColorRGBA(128, 128, 128, 255));
	mat->SetMetallic(0.9);
	mat->SetRoughness(0.4);
	mat->SetAmbientOcclusion(1.0);
	auto lightColor = pointLight->GetLightColor();
	mat->SetEmissive(ColorRGBA(lightColor.x() * 255, lightColor.y() * 255, lightColor.z() * 255, std::min(int(pointLight->GetInstensive() * 255), 255)));
	
	scene->SetOnTick([=](float deltaTime)
		{
			auto rotation = Quaternionf(AngleAxisf(deltaTime * 3.14 * 0.3, Vector3f::UnitY()));
			pointLightNode->GetTransform()->SetPosition(rotation * pointLightNode->GetTransform()->Position());
		});

}
