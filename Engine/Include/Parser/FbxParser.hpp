#include <string>
#include "Common/Scene.hpp"
#include "../../3rdparty/open_fbx/ofbx.h"
#include "Graphic/Vertex.hpp"
#include "Graphic/StaticMeshComponent.hpp"
#include "Graphic/Mesh.hpp"
#include "Utils/Debug.hpp"
namespace Entropy{
	std::shared_ptr<StaticMeshComponent> ParseFBX(std::string path) {
		FILE* fp = fopen(path.c_str(), "rb");
		if (!fp) return false;

		fseek(fp, 0, SEEK_END);
		long file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		auto* content = new ofbx::u8[file_size];
		fread(content, 1, file_size, fp);
		auto g_scene = ofbx::load((ofbx::u8*)content, file_size, (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
		if (!g_scene) {
			Log(ofbx::getError());
		}
		auto staticMeshComponent = std::make_shared<StaticMeshComponent>();
		staticMeshComponent->Initialize();

		for (int m = 0; m < g_scene->getMeshCount(); m++) {
			auto mesh = std::make_shared<Mesh>();
			auto geometry = g_scene->getMesh(m)->getGeometry();
			std::vector<Vertex>* vertices = new std::vector<Vertex>(geometry->getVertexCount());

			for (int i = 0; i < geometry->getVertexCount(); i ++) {
				Vertex vertex;

				auto v = geometry->getVertices()[i];
				vertex.m_x = v.x;
				vertex.m_y = v.y;
				vertex.m_z = v.z;

				auto t = geometry->getUVs()[i];
				vertex.m_tx = t.x;
				vertex.m_ty = 1- t.y;

				auto n = geometry->getNormals()[i];
				vertex.m_nx = n.x;
				vertex.m_ny = n.y;
				vertex.m_nz = n.z;

				vertices->at(i) = vertex;
			}

			std::vector<uint32_t>* indices = new std::vector<uint32_t>(geometry->getIndexCount());

			for (int i = 0; i < geometry->getIndexCount(); i++) {
				// indices->at(i) = (uint32_t)geometry->getFaceIndices()[i];
				indices->at(i) = i;
			}
			
			mesh->m_vertexBuffer = &(*vertices)[0];
			mesh->m_vertexCount = vertices->size();
			mesh->m_vertexBufferSize = vertices->size() * sizeof(Vertex);

			//
			mesh->m_indexBuffer = &(*indices)[0];
			mesh->m_indexCount = indices->size();
			mesh->m_indexBufferSize = indices->size() * sizeof(uint32_t);
			staticMeshComponent->GetMeshes()->push_back(mesh);
		}

		return staticMeshComponent;
	}
}
