#include "EntropyApp.hpp"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
#include "Common/Transform.hpp"
#include "Eigen/Core"	
#include "Utils/Debug.hpp"
#include "Parser/ObjLoader.hpp"
using namespace Entropy;

struct Vertex
{
	float m_x;
	float m_y;
	float m_z;
	
	// float m_nx;
	// float m_ny;
	// float m_nz;
	//
	// float m_tx;
	// float m_ty;
};

int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();
	auto cubeNode = std::shared_ptr<SceneGeometryNode>(std::make_shared<SceneGeometryNode>());
	auto mesh = std::make_shared<Mesh>();

	objl::Loader loader;
	loader.LoadFile("Assets/bunny.obj");

	// static PosColorVertex s_cubeVertices[] =
	// {
	// 	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	// 	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	// 	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	// 	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	// 	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	// 	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	// 	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	// 	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
	// };

	Vertex* vertices = new Vertex[loader.LoadedVertices.size()];
	for(int i = 0 ;i < loader.LoadedVertices.size() ;i++ ) {
		vertices[i].m_x = loader.LoadedVertices[i].Position.X;
		vertices[i].m_y = loader.LoadedVertices[i].Position.Y;
		vertices[i].m_z = loader.LoadedVertices[i].Position.Z;
	}

	
	mesh->m_vertexBuffer = vertices;
	
	mesh->m_vertexCount = loader.LoadedVertices.size();
	// mesh->m_vertexBufferSize = sizeof(vertices);
	mesh->m_vertexBufferSize = loader.LoadedVertices.size() * 4 * 3;

	// uint16_t i[] = {
	// 	0, 1, 2,
	// 	3,
	// 	7,
	// 	1,
	// 	5,
	// 	0,
	// 	4,
	// 	2,
	// 	6,
	// 	7,
	// 	4,
	// 	5,
	// };
	mesh->m_indexBuffer = &loader.LoadedIndices[0];
	mesh->m_indexCount = loader.LoadedIndices.size();
	// mesh->m_indexBufferSize = sizeof(loader.LoadedIndices);
	mesh->m_indexBufferSize = loader.LoadedIndices.size() * 4;
	cubeNode->m_Mesh.push_back(mesh);

	scene->Geometries.push_back(cubeNode);
	scene->SetOnTick([&cubeNode](float deltaTime)
		{
			cubeNode->GetTransform()->Rotate(deltaTime *3.14 * 0.1,Vector3f(1, 1, 1));
		});
	scene->SceneGraph->AppendChild(cubeNode);
	
	auto cameraNode = std::shared_ptr<Camera>(std::make_shared<Camera>());
	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0,0,-10));
	
	scene->MainCamera = cameraNode;
	scene->Cameras.push_back(cameraNode);
	scene->SceneGraph->AppendChild(std::move(cameraNode));

	return app.run(_argc, _argv);
}