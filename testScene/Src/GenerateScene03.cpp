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

void GenerateScene03(Entropy::Scene* scene) {
	auto skybox = std::make_shared<SkyBox>();
	skybox->SetHdrTexture(std::shared_ptr<Texture>(
		new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/cubemap.dds", bimg::TextureFormat::RGB8)))));
	scene->SetSkybox(skybox);

	auto camera = std::shared_ptr<Camera>(std::make_shared<Camera>());
	// camera->SetProjectionType(Ortho);
	auto cameraNode = std::make_shared<SceneNode>("Camera");
	cameraNode->Initialize();

	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0, 0, -2));
	// cameraNode->GetTransform()->Rotate(0.6, Vector3f::UnitX());
	cameraNode->AddComponent(camera);
	scene->MainCamera = camera;
	scene->GetRootNode()->AddChild(cameraNode->SharedPtr());

	auto ibl_irradiance = "Textures/irradiance.dds";
	auto ibl_prefilter = "Textures/prefilter.dds";
	auto ibl_brdf = "Textures/brdf.png";

	auto sphereMesh = GenerateSphere(0.3f,128, 128);
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
		std::shared_ptr<SceneNode> node(new SceneNode("Grass"));
		node->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/grass/albedo.png", bimg::TextureFormat::RGB8))));
		mat->SetMetallic(0.);
		mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/grass/roughness.png", bimg::TextureFormat::RGB8))));
		mat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/grass/ao.png", bimg::TextureFormat::RGB8))));
		mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/grass/normal.png", bimg::TextureFormat::RGB8))));
		mat->SetIBL(
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_irradiance, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_prefilter, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_brdf, bimg::TextureFormat::RGBA8)))
		);
		node->GetTransform()->SetPosition(Vector3f(-1, 0, 0));
		node->AddComponent(meshComponent->SharedPtr());

		scene->GetRootNode()->AddChild(node->SharedPtr());
	}
	{
		std::shared_ptr<SceneNode> node(new SceneNode("Plastic"));
		node->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/plastic/albedo.png", bimg::TextureFormat::RGB8))));
		mat->SetMetallic(0.);
		mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/plastic/roughness.png", bimg::TextureFormat::RGB8))));
		mat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/plastic/ao.png", bimg::TextureFormat::RGB8))));
		// mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/plastic/normal.png", bimg::TextureFormat::RGB8))));
		mat->SetIBL(
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_irradiance, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_prefilter, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_brdf, bimg::TextureFormat::RGBA8)))
		);
		node->GetTransform()->SetPosition(Vector3f(0, 0, 0));
		node->AddComponent(meshComponent->SharedPtr());

		scene->GetRootNode()->AddChild(node->SharedPtr());
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
		// mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/plastic/normal.png", bimg::TextureFormat::RGB8))));
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
		std::shared_ptr<SceneNode> node(new SceneNode("Wall"));
		node->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/wall/albedo.png", bimg::TextureFormat::RGB8))));
		mat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/wall/metallic.png", bimg::TextureFormat::RGB8))));
		mat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/wall/roughness.png", bimg::TextureFormat::RGB8))));
		mat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/wall/ao.png", bimg::TextureFormat::RGB8))));
		mat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/wall/normal.png", bimg::TextureFormat::RGB8))));
		mat->SetIBL(
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_irradiance, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_prefilter, bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad(ibl_brdf, bimg::TextureFormat::RGBA8)))
		);
		node->GetTransform()->SetPosition(Vector3f(2, 0, 0));
		node->AddComponent(meshComponent->SharedPtr());

		scene->GetRootNode()->AddChild(node->SharedPtr());
	}
}


