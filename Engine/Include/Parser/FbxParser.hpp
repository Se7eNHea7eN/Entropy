#include <string>
#include "Common/Scene.hpp"
#include "../../3rdparty/open_fbx/ofbx.h"
#include "Graphic/Vertex.hpp"
namespace Entropy{
	std::shared_ptr<SceneGeometryNode> ParseFBX(std::string path) {
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
		auto objNode = std::shared_ptr<SceneGeometryNode>(std::make_shared<SceneGeometryNode>());
		for (int m = 0; m < g_scene->getMeshCount(); m++) {
			auto mesh = std::make_shared<Mesh>();
			auto geometry = g_scene->getMesh(m)->getGeometry();
			std::vector<Vertex>* vertices = new std::vector<Vertex>(geometry->getVertexCount());

			for (int offset = 0; offset < geometry->getVertexCount(); offset ++) {
				Vertex vertex;

				auto v = geometry->getVertices()[offset];
				vertex.m_x = v.x;
				vertex.m_y = v.y;
				vertex.m_z = v.z;

				auto t = geometry->getUVs()[offset];
				vertex.m_tx = t.x;
				vertex.m_ty = t.y;

				auto n = geometry->getNormals()[offset];
				vertex.m_nx = n.x;
				vertex.m_ny = n.y;
				vertex.m_nz = n.z;

				vertices->at(offset) = vertex;
			}

			std::vector<uint32_t>* indices = new std::vector<uint32_t>(geometry->getIndexCount());
			
			mesh->m_vertexBuffer = &(*vertices)[0];
			mesh->m_vertexCount = vertices->size();
			mesh->m_vertexBufferSize = vertices->size() * sizeof(Vertex);

			//
			mesh->m_indexBuffer = geometry->getFaceIndices();
			mesh->m_indexCount = geometry->getIndexCount();
			mesh->m_indexBufferSize = geometry->getIndexCount() * sizeof(int);
			objNode->m_Mesh.push_back(mesh);
		}

		return objNode;
	}
}
