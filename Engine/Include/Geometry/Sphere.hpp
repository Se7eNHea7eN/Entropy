#include "Graphic/Mesh.hpp"
#include "Graphic/Vertex.hpp"
#include <memory>
#include <cmath>
#include <vector>

namespace Entropy {

	std::shared_ptr<Mesh> GenerateSphere(float radius = 1, uint16_t sectorCount = 64, uint16_t stackCount = 64) {
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		std::vector<Vertex>* vertices = new std::vector<Vertex>;
		std::vector<uint32_t>* indices = new std::vector<uint32_t>;

		const unsigned int X_SEGMENTS = sectorCount;
		const unsigned int Y_SEGMENTS = sectorCount;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {

				Vertex v;
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				v.m_nx = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				v.m_ny = std::cos(ySegment * PI);
				v.m_nz = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				v.m_tx = xSegment;
				v.m_ty = ySegment;
				v.m_x = v.m_nx * radius;
				v.m_y = v.m_ny * radius;
				v.m_z = v.m_nz * radius;
				vertices->push_back(v);
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y) {
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x) {
					indices->push_back(y * (X_SEGMENTS + 1) + x);
					indices->push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else {
				for (int x = X_SEGMENTS; x >= 0; --x) {
					indices->push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices->push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}


		mesh->m_vertexBuffer = &(*vertices)[0];
		mesh->m_vertexCount = vertices->size();
		mesh->m_vertexBufferSize = vertices->size() * sizeof(Vertex);

		//	
		mesh->m_indexBuffer = &(*indices)[0];
		mesh->m_indexCount = indices->size();
		mesh->m_indexBufferSize = indices->size() * sizeof(uint32_t);
		mesh->m_indiceType = UINT64_C(0x0001000000000000);


		return mesh;
	}


	
}
