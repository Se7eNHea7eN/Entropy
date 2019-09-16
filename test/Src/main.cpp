#include "EntropyApp.hpp"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
#include "Common/Transform.hpp"
#include "Eigen/Core"	
#include "Utils/Debug.hpp"
using namespace Entropy;

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;
};

int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();
	auto cubeNode = std::shared_ptr<SceneGeometryNode>(std::make_shared<SceneGeometryNode>());
	auto mesh = std::make_shared<Mesh>();

	static PosColorVertex s_cubeVertices[] =
	{
		{-1.0f,  1.0f,  1.0f, 0xff000000 },
		{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
		{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
		{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
		{-1.0f,  1.0f, -1.0f, 0xffff0000 },
		{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
		{-1.0f, -1.0f, -1.0f, 0xffffff00 },
		{ 1.0f, -1.0f, -1.0f, 0xffffffff },
	};

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
	mesh->m_vertexBuffer = s_cubeVertices;
	
	mesh->m_vertexCount = 8;
	mesh->m_vertexBufferSize = sizeof(s_cubeVertices);
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
	mesh->m_indexBufferSize = sizeof(i);
	cubeNode->m_Mesh.push_back(mesh);

	scene->Geometries.push_back(cubeNode);
	scene->SetOnTick([&cubeNode](float deltaTime)
		{
			cubeNode->GetTransform()->Rotate(deltaTime *3.14,Vector3f(1, 1, 1));
		});
	scene->SceneGraph->AppendChild(cubeNode);
	
	auto cameraNode = std::shared_ptr<Camera>(std::make_shared<Camera>());
	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0,0,-35));
	
	scene->MainCamera = cameraNode;
	scene->Cameras.push_back(cameraNode);
	scene->SceneGraph->AppendChild(std::move(cameraNode));

	return app.run(_argc, _argv);
}