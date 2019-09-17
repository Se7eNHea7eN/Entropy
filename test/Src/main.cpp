#include "EntropyApp.hpp"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
#include "Common/Transform.hpp"
#include "Eigen/Core"	
#include "Utils/Debug.hpp"
#include "Parser/ObjParser.hpp"

using namespace Entropy;

int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();

	std::string inputfile = "Assets/bunny.obj";
	// std::string inputfile = "Assets/dragon.obj";
	auto objNode = ParseObj(inputfile);
	auto material = std::make_shared<Material>();
	objNode->m_Materials.push_back(material);
	material->SetName("PBR");
	material->SetVertexShader("vs_common");
	material->SetFragmentShader("fs_pbr");
	
	// cubeNode->GetTransform()->SetScale(Vector3f(0.5, 0.5, 0.5));
	scene->Geometries.push_back(objNode);
	scene->SetOnTick([&objNode](float deltaTime)
		{
			objNode->GetTransform()->Rotate(deltaTime *3.14 * 0.1,Vector3f::UnitY());
		});
	scene->SceneGraph->AppendChild(objNode);
	
	auto cameraNode = std::shared_ptr<Camera>(std::make_shared<Camera>());
	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0,2,-5));
	cameraNode->SetTarget(objNode->GetTransform());
	scene->MainCamera = cameraNode;
	scene->Cameras.push_back(cameraNode);
	scene->SceneGraph->AppendChild(std::move(cameraNode));

	return app.run(_argc, _argv);
}