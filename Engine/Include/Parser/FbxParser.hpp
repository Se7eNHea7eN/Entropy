#include <string>
#include "Common/Scene.hpp"
#include "../../3rdparty/open_fbx/ofbx.h"
#include "Graphic/Vertex.hpp"
#include "Graphic/StaticMeshComponent.hpp"
#include "Graphic/Mesh.hpp"
#include "Utils/Debug.hpp"
namespace Entropy{
	std::vector<std::shared_ptr<Mesh>>* ParseFBX(std::string path) {
		FILE* fp = fopen(path.c_str(), "rb");
		if (!fp) {
			Log("open file faild!");
		}

		fseek(fp, 0, SEEK_END);
		long file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		auto* content = new ofbx::u8[file_size];
		fread(content, 1, file_size, fp);
		auto g_scene = ofbx::load((ofbx::u8*)content, file_size, (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
		if (!g_scene) {
			Log(ofbx::getError());
		}

		auto meshes = new std::vector<std::shared_ptr<Mesh>>;

		for (int m = 0; m < g_scene->getMeshCount(); m++) {
			auto mesh = std::make_shared<Mesh>();
			auto geometry = g_scene->getMesh(m)->getGeometry();
			mesh->vertices.resize(geometry->getVertexCount());

			for (int i = 0; i < geometry->getVertexCount(); i ++) {
				Vertex vertex;

				auto v = geometry->getVertices()[i];
				vertex.x = v.x;
				vertex.y = v.y;
				vertex.z = v.z;

				auto uv= geometry->getUVs()[i];
				vertex.texCoordX = uv.x;
				vertex.texCoordY = 1- uv.y;

				auto n = geometry->getNormals()[i];
				vertex.normalX = n.x;
				vertex.normalY = n.y;
				vertex.normalZ = n.z;

				mesh->vertices.at(i) = vertex;
			}
			mesh->CalculateTangents();
			meshes->push_back(mesh);
		}

		return meshes;
	}
}
