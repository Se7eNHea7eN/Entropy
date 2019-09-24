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
	std::string inputfile = "Assets/Rifle_2.fbx";

	std::shared_ptr<SceneNode> objNode = std::shared_ptr<SceneNode>(std::make_shared<SceneNode>());
	
	std::shared_ptr<StaticMeshComponent> meshComponent;
	if(ends_with(inputfile,".obj")) {
		meshComponent = ParseObj(inputfile);
	}else if(ends_with(inputfile, ".fbx")) {
		meshComponent = ParseFBX(inputfile);
	}else {
		return -1;
	}
	
	auto material = std::make_shared<Material>();
	meshComponent->GetMaterials()->push_back(material);
	material->SetName("PBR");
	material->SetVertexShader("vs_common");
	material->SetFragmentShader("fs_pbr");
	// material->SetFragmentShader("fs_lighting");
	// material->m_Albedo = ColorRGBA(51, 255, 0,255);
	// material->m_Albedo = ColorRGBA(0, 0, 255,255);
	material->m_Albedo = std::make_shared<Texture>(Texture(0, imageLoad("Textures/Rifle_2_Albedo.jpg", bimg::TextureFormat::RGBA8)));
	// material->m_Metallic = 0.85f;
	material->m_Metallic = std::make_shared<Texture>(Texture(0, imageLoad("Textures/Rifle_2_Metallic.png", bimg::TextureFormat::R8)));
	// material->m_Roughness = 0.4f;
	material->m_Roughness = std::make_shared<Texture>(Texture(0,imageLoad("Textures/Rifle_2_Roughness.png", bimg::TextureFormat::R8)));
	material->m_Normal = std::make_shared<Texture>(Texture(0, imageLoad("Textures/Rifle_2_Normal.jpg", bimg::TextureFormat::R8)));
	material->m_AmbientOcclusion = std::make_shared<Texture>(Texture(0,imageLoad("Textures/Rifle_2_AO.png", bimg::TextureFormat::R8)));

	// material->m_AmbientOcclusion = 1.0f;
	objNode->GetTransform()->SetScale(Vector3f(0.02, 0.02,0.02));
	// objNode->GetTransform()->SetScale(Vector3f(16, 16,16));
	scene->SetOnTick([&objNode](float deltaTime)
		{
			objNode->GetTransform()->Rotate(deltaTime *3.14 * 0.1,Vector3f::UnitY());
		});

	objNode->AppendComponent(meshComponent);
	
	auto cameraNode = std::shared_ptr<Camera>(std::make_shared<Camera>());
	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0,2,-5));
	cameraNode->GetTransform()->Rotate(0.2, Vector3f::UnitX());
	// cameraNode->GetTransform()->Rotate(0.4, Vector3f::UnitX());
	// 
	// cameraNode->SetTarget(objNode->GetTransform());
	scene->MainCamera = cameraNode;
	// scene->Cameras.push_back(cameraNode);
	scene->GetRootNode()->AppendChild(std::move(objNode));
	scene->GetRootNode()->AppendChild(std::move(cameraNode));
	try {
		return app.run(_argc, _argv);

	}catch(std::exception e) {
		Log(e.what());
	}
}