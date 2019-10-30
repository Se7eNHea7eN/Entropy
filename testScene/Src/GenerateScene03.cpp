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
void GenerateScene03(Entropy::Scene* scene) {
	// auto skybox = std::make_shared<SkyBox>();
	// skybox->SetHdrTexture(std::shared_ptr<Texture>(
	// 	new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/cubemap.dds", bimg::TextureFormat::RGB8)))));
	// scene->SetSkybox(skybox);

	auto camera = std::shared_ptr<Camera>(std::make_shared<Camera>());
	camera->SetFovY(PI /8);
	auto cameraNode = std::make_shared<SceneNode>("Camera");
	cameraNode->Initialize();

	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0, 0, -6.5));
	// cameraNode->GetTransform()->Rotate(0.6, Vector3f::UnitX());
	cameraNode->AddComponent(camera);
	scene->MainCamera = camera;
	scene->GetRootNode()->AddChild(cameraNode->SharedPtr());

	auto ibl_irradiance = "Textures/irradiance.dds";
	auto ibl_prefilter = "Textures/prefilter.dds";
	auto ibl_brdf = "Textures/brdf.png";

	auto sphereMesh = GenerateSphere(0.3f,128, 128);
	sphereMesh->CalculateTangents();
	{
		std::shared_ptr<SceneNode> node(new SceneNode("Gold"));
		node->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/gold/albedo.png", bimg::TextureFormat::RGB8))));
		mat->SetMetallic(1);
		mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/gold/roughness.png", bimg::TextureFormat::RGB8))));
		mat->SetAmbientOcclusion(1);
		mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/gold/normal.png", bimg::TextureFormat::RGB8))));
		mat->SetIBL(
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_irradiance, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_prefilter, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_brdf, bimg::TextureFormat::RGBA8)))
		);
		node->GetTransform()->SetPosition(Vector3f(-2, 0, 0));
		node->AddComponent(meshComponent->SharedPtr());

		scene->GetRootNode()->AddChild(node->SharedPtr());
	}
	{
		std::shared_ptr<SceneNode> node(new SceneNode("tile2"));
		node->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile2/albedo.png", bimg::TextureFormat::RGB8))));
		// mat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile2/metallic.png", bimg::TextureFormat::RGB8))));
		mat->SetMetallic(0.3);
		mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile2/roughness.png", bimg::TextureFormat::RGB8))));
		mat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile2/ao.png", bimg::TextureFormat::RGB8))));
		mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile2/normal.png", bimg::TextureFormat::RGB8))));
		mat->SetDepthMap(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile2/height.png", bimg::TextureFormat::RGB8))),
			1);
		mat->SetIBL(
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_irradiance, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_prefilter, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_brdf, bimg::TextureFormat::RGBA8)))
		);
		node->GetTransform()->SetPosition(Vector3f(-1, 0, 0));
		node->AddComponent(meshComponent->SharedPtr());

		scene->GetRootNode()->AddChild(node->SharedPtr());
	}

	std::shared_ptr<SceneNode> tilenode(new SceneNode("tile"));


	{
		tilenode->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile/albedo.png", bimg::TextureFormat::RGB8))));
		mat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile/metallic.png", bimg::TextureFormat::RGB8))));
		mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile/roughness.png", bimg::TextureFormat::RGB8))));
		mat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile/ao.png", bimg::TextureFormat::RGB8))));
		mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile/normal.png", bimg::TextureFormat::RGB8))));
		mat->SetDepthMap(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/tile/height.png", bimg::TextureFormat::RGB8))),
			1);
		mat->SetIBL(
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_irradiance, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_prefilter, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_brdf, bimg::TextureFormat::RGBA8)))
		);
		tilenode->GetTransform()->SetPosition(Vector3f(0, 0, 0));
		tilenode->AddComponent(meshComponent->SharedPtr());

		scene->GetRootNode()->AddChild(tilenode->SharedPtr());
	}

	{
		std::shared_ptr<SceneNode> node(new SceneNode("Iron"));
		node->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/rusted_iron/albedo.png", bimg::TextureFormat::RGB8))));
		mat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/rusted_iron/metallic.png", bimg::TextureFormat::RGB8))));
		mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/rusted_iron/roughness.png", bimg::TextureFormat::RGB8))));
		mat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/rusted_iron/ao.png", bimg::TextureFormat::RGB8))));
		mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/rusted_iron/normal.png", bimg::TextureFormat::RGB8))));
		mat->SetIBL(
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_irradiance, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_prefilter, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_brdf, bimg::TextureFormat::RGBA8)))
		);
		node->GetTransform()->SetPosition(Vector3f(1, 0, 0));
		node->AddComponent(meshComponent->SharedPtr());

		scene->GetRootNode()->AddChild(node->SharedPtr());
	}

	{
		std::shared_ptr<SceneNode> node(new SceneNode("cloth"));
		node->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/cloth2/albedo.png", bimg::TextureFormat::RGB8))));
		// mat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/rock/metallic.png", bimg::TextureFormat::RGB8))));
		mat->SetMetallic(0.0);
		mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/cloth2/roughness.png", bimg::TextureFormat::RGB8))));
		mat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/cloth2/ao.png", bimg::TextureFormat::RGB8))));
		mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/cloth2/normal.png", bimg::TextureFormat::RGB8))));
		mat->SetDepthMap(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/cloth2/height.png", bimg::TextureFormat::RGB8))),
			0.05);
		mat->SetIBL(
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_irradiance, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_prefilter, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_brdf, bimg::TextureFormat::RGBA8)))
		);

		node->GetTransform()->SetPosition(Vector3f(2, 0, 0));
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
			tilenode->GetTransform()->Rotate(-deltaTime * 3.14 * 0.1, Vector3f::UnitY());
		});
}


