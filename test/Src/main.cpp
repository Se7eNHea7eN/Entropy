#include "EntropyApp.hpp"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
#include "Common/SceneObject.hpp"
#include "Common/Transform.hpp"
#include "Eigen/Core"
using namespace Entropy;
int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();
	auto cubeNode = std::unique_ptr<SceneGeometryNode>(std::make_unique<SceneGeometryNode>());
	auto _object = std::make_shared<SceneObjectGeometry>();
	auto mesh = std::make_shared<Mesh>();
	float v[] =
	{
		-1.0f,  1.0f,  1.0f ,
		 1.0f,  1.0f,  1.0f ,
		-1.0f, -1.0f,  1.0f ,
		 1.0f, -1.0f,  1.0f ,
		-1.0f,  1.0f, -1.0f ,
		 1.0f,  1.0f, -1.0f ,
		-1.0f, -1.0f, -1.0f ,
		 1.0f, -1.0f, -1.0f 
	};
	mesh->m_vertexBuffer = v;

	mesh->m_vertexCount = 8 * 3;

	uint16_t i[] = {
		0, 1, 2,
		3,
		7,
		1,
		5,
		0,
		4,
		2,
		6,
		7,
		4,
		5,
	};
	mesh->m_indexBuffer = i;
	mesh->m_indexCount = 14;
	
	_object->AddMesh(mesh);
	scene->Geometries.push_back(_object);
	cubeNode->AddSceneObjectRef(_object);

	// scene->SetOnTick([&cubeNode]()
	// 	{
	// 		cubeNode->m_Transform->Isometry().rotate()
	// 	});
	scene->SceneGraph->AppendChild(std::move(cubeNode));


	auto cameraNode = std::shared_ptr<SceneObjectCamera>(std::make_shared<SceneObjectCamera>());
	cameraNode->m_Transform->Isometry().translate(Vector3f(0,0,-35));

	scene->MainCamera = cameraNode;
	scene->Cameras.push_back(cameraNode);
	scene->SceneGraph->AppendChild(std::move(cameraNode));

	return app.run(_argc, _argv);
}