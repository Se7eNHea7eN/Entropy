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

void GenerateScene02(Entropy::Scene* scene) {
	auto skybox = std::make_shared<SkyBox>();
	skybox->SetHdrTexture(std::shared_ptr<Texture>(
		new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/cubemap.dds", bimg::TextureFormat::RGB8)))));
	scene->SetSkybox(skybox);

	auto camera = std::shared_ptr<Camera>(std::make_shared<Camera>());
	auto cameraNode = std::make_shared<SceneNode>("Camera");
	cameraNode->Initialize();

	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0, 0, -4));
	// cameraNode->GetTransform()->Rotate(0.6, Vector3f::UnitX());
	cameraNode->AddComponent(camera);
	scene->MainCamera = camera;
	scene->GetRootNode()->AddChild(cameraNode->SharedPtr());


	auto sphereMesh = GenerateSphere(0.3f,128, 128);
	for (int i = 0; i < 8; i++) {
		std::shared_ptr<SceneNode> node(new SceneNode("Sphere"));
		node->Initialize();
		std::shared_ptr<StaticMeshComponent> meshComponent(new StaticMeshComponent());
		meshComponent->Initialize();
		meshComponent->GetMeshes().push_back(sphereMesh);
		auto mat = std::make_shared<StandardPBRMaterial>();
		meshComponent->GetMaterials().push_back(mat);
		mat->SetAlbedo(ColorRGBA(128, 128, 128, 255));

		mat->SetMetallic(1 - i/8.f);
		mat->SetRoughness(i / 8.f);
		mat->SetAmbientOcclusion(1);
		mat->SetIBL(
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/irradiance.dds", bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/prefilter.dds", bimg::TextureFormat::RGB8))),
			new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Textures/brdf.png", bimg::TextureFormat::RGBA8)))
		);
		node->GetTransform()->SetPosition(Vector3f(i-4, 0, 0));
		node->AddComponent(meshComponent->SharedPtr());

		scene->GetRootNode()->AddChild(node->SharedPtr());
	}
}
