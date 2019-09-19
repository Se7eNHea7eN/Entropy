#include "EntropyApp.hpp"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
#include "Common/Transform.hpp"
#include "Eigen/Core"	
#include "Utils/Debug.hpp"
#include "Parser/ObjParser.hpp"
#include "Parser/BMP.hpp"

using namespace Entropy;

int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();
	// std::string inputfile = "Assets/bunny.obj";
	std::string inputfile = "Assets/dragon.obj";
	// std::string inputfile = "Assets/cube.obj";
	
	auto objNode = ParseObj(inputfile);
	auto material = std::make_shared<Material>();
	objNode->m_Materials.push_back(material);
	material->SetName("PBR");
	material->SetVertexShader("vs_common");
	material->SetFragmentShader("fs_pbr");
	// material->SetFragmentShader("fs_lighting");
	// material->m_Albedo = ColorRGBA(51, 255, 0,255);
	material->m_Albedo = ColorRGBA(0, 0, 255,255);
	material->m_Metallic = 0.85f;
	material->m_Roughness = 0.4f;
	material->m_AmbientOcclusion = 1.0f;
	objNode->GetTransform()->SetScale(Vector3f(16,16,16));
	scene->Geometries.push_back(objNode);
	scene->SetOnTick([&objNode](float deltaTime)
		{
			objNode->GetTransform()->Rotate(deltaTime *3.14 * 0.1,Vector3f::UnitY());
		});
	scene->SceneGraph->AppendChild(objNode);
	
	auto cameraNode = std::shared_ptr<Camera>(std::make_shared<Camera>());
	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0,2,-5));
	// cameraNode->GetTransform()->Rotate(0.2, Vector3f::UnitX());
	// cameraNode->GetTransform()->Rotate(0.4, Vector3f::UnitX());
	// 
	// cameraNode->SetTarget(objNode->GetTransform());
	scene->MainCamera = cameraNode;
	scene->Cameras.push_back(cameraNode);
	scene->SceneGraph->AppendChild(std::move(cameraNode));

	return app.run(_argc, _argv);
}