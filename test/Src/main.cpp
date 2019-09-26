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

using namespace Entropy;

int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();
	// std::string inputfile = "Assets/bunny.obj";
	// std::string inputfile = "Assets/dragon.obj";
	// std::string inputfile = "Assets/gun.obj";
	// std::string inputfile = "Assets/cube.obj";
	std::string inputfile = "Assets/Rifle2/Rifle_2.fbx";

	std::shared_ptr<SceneNode> objNode(new SceneNode());
	
	std::shared_ptr<StaticMeshComponent> rifleMeshComponent(new StaticMeshComponent());
	rifleMeshComponent->Initialize();

	std::vector<std::shared_ptr<Mesh>>* meshes;
	
	if(ends_with(inputfile,".obj")) {
		meshes = ParseObj(inputfile);
	}else if(ends_with(inputfile, ".fbx")) {
		meshes = ParseFBX(inputfile);
	}else {
		return -1;
	}

	for(auto m : *meshes) {
		rifleMeshComponent->GetMeshes().push_back(m);
	}
	auto rifleMat = std::make_shared<StandardPBRMaterial>();
	rifleMeshComponent->GetMaterials().push_back(rifleMat);
	
	rifleMat->SetAlbedo(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Albedo.jpg", bimg::TextureFormat::RGB8))));
	rifleMat->SetNormal(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Normal.jpg", bimg::TextureFormat::RGB8))));
	rifleMat->SetMetallic(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Metallic.jpg", bimg::TextureFormat::R8))));
	rifleMat->SetRoughness(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_Roughness.jpg", bimg::TextureFormat::R8))));
	rifleMat->SetAmbientOcclusion(new Texture(std::shared_ptr<bimg::ImageContainer>(imageLoad("Assets/Rifle2/Rifle_2_AO.jpg", bimg::TextureFormat::R8))));

	objNode->GetTransform()->SetScale(Vector3f(0.02, 0.02,0.02));
	// objNode->GetTransform()->SetScale(Vector3f(16, 16,16));
	scene->SetOnTick([&objNode](float deltaTime)
		{
			objNode->GetTransform()->Rotate(deltaTime *3.14 * 0.1,Vector3f::UnitY());
		});

	objNode->AppendComponent(rifleMeshComponent->SharedPtr());
	
	auto camera = std::shared_ptr<Camera>(std::make_shared<Camera>());
	auto cameraNode = std::shared_ptr<SceneNode>(std::make_shared<SceneNode>());
	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0,2,-5));
	cameraNode->GetTransform()->Rotate(0.3, Vector3f::UnitX());
	cameraNode->AppendComponent(camera);
	// cameraNode->SetTarget(objNode->GetTransform());
	scene->MainCamera = camera;
	// scene->Cameras.push_back(cameraNode);
	scene->GetRootNode()->AppendChild(objNode->SharedPtr());
	scene->GetRootNode()->AppendChild(cameraNode->SharedPtr());
	try {
		return app.run(_argc, _argv);

	}catch(std::exception e) {
		Log(e.what());
	}
}