#include "EntropyApp.hpp"
#include "Common/Scene.hpp"
#include "Common/SceneNode.hpp"
#include "Common/Transform.hpp"
#include "Eigen/Core"	
#include "Utils/Debug.hpp"
#include "../../Engine/3rdparty/tiny_obj_loader/tiny_obj_loader.h"

using namespace Entropy;

namespace Entropy {
	struct Vertex
	{
		float m_x;
		float m_y;
		float m_z;

		float m_nx;
		float m_ny;
		float m_nz;
		//
		// float m_tx;
		// float m_ty;
	};

}


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
//
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
int main(int _argc, const char* const* _argv)
{
	EntropyApp app;
	app.entropyCore->CreateScene("Test Scene");
	auto scene = app.entropyCore->CurrentScene();
	auto cubeNode = std::shared_ptr<SceneGeometryNode>(std::make_shared<SceneGeometryNode>());

	// std::string inputfile = "Assets/bunny.obj";
	std::string inputfile = "Assets/dragon.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (!warn.empty()) {
		Log(warn.c_str());
	}

	if (!err.empty()) {
		Log(err.c_str());
	}

	if (!ret) {
		exit(1);
	}

	
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {

		auto mesh = std::make_shared<Mesh>();

		std::vector<Vertex>* vertices = new std::vector<Vertex>;
		std::vector<uint32_t>* indices = new std::vector<uint32_t>;
		// uint16_t index = 0;
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				indices->push_back(indices->size());

				Vertex vertex;
				vertex.m_x = attrib.vertices[3 * idx.vertex_index + 0];
				vertex.m_y = attrib.vertices[3 * idx.vertex_index + 1];
				vertex.m_z = attrib.vertices[3 * idx.vertex_index + 2];
				vertex.m_nx = attrib.normals[3 * idx.normal_index + 0];
				vertex.m_ny = attrib.normals[3 * idx.normal_index + 1];
				vertex.m_nz = attrib.normals[3 * idx.normal_index + 2];
				vertices->push_back(vertex);
				// tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				// tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
			}
			index_offset += fv;

			// per-face material
			// shapes[s].mesh.material_ids[f];
		}

		mesh->m_vertexBuffer = &(*vertices)[0];
		mesh->m_vertexCount = vertices->size();
		mesh->m_vertexBufferSize = vertices->size() * 4 * 6;

		//
		mesh->m_indexBuffer = &(*indices)[0];
		mesh->m_indexCount = indices->size();
		mesh->m_indexBufferSize = indices->size() * 4;
		cubeNode->m_Mesh.push_back(mesh);
	}
	
	// cubeNode->GetTransform()->SetScale(Vector3f(0.5, 0.5, 0.5));
	scene->Geometries.push_back(cubeNode);
	scene->SetOnTick([&cubeNode](float deltaTime)
		{
			cubeNode->GetTransform()->Rotate(deltaTime *3.14 * 0.1,Vector3f::UnitY());
		});
	scene->SceneGraph->AppendChild(cubeNode);
	
	auto cameraNode = std::shared_ptr<Camera>(std::make_shared<Camera>());
	cameraNode->GetTransform()->Translate(Eigen::Vector3f(0,0,-5));
	
	scene->MainCamera = cameraNode;
	scene->Cameras.push_back(cameraNode);
	scene->SceneGraph->AppendChild(std::move(cameraNode));

	return app.run(_argc, _argv);
}