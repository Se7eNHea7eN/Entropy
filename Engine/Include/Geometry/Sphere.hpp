#include "Graphic/Mesh.hpp"
#include "Graphic/Vertex.hpp"
#include <memory>
#include <cmath>
#include <vector>

namespace Entropy {

	std::shared_ptr<Mesh> GenerateSphere(float radius = 1,uint16_t sectorCount = 64, uint16_t stackCount = 64) {
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		std::vector<Vertex>* vertices = new std::vector<Vertex>;
		std::vector<uint32_t>* indices= new std::vector<uint32_t>;

		float x, y, z, xy;                              // vertex position
		float lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord
		const float PI = 3.14159265359;

		float sectorStep = 2 * PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				Vertex v;
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				v.m_x = x;
				v.m_y = y;
				v.m_z = z;

				// normalized vertex normal (nx, ny, nz)
				v.m_nx = x * lengthInv;
				v.m_ny = y * lengthInv;
				v.m_nz = z * lengthInv;
			

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				v.m_tx = s;
				v.m_ty = t;
				vertices->push_back(v);
			}
		}


		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices->push_back(k1);
					indices->push_back(k2);
					indices->push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					indices->push_back(k1 + 1);
					indices->push_back(k2);
					indices->push_back(k2 + 1);
				}
			}
		}

		mesh->m_vertexBuffer = &(vertices)[0];
		mesh->m_vertexCount = vertices->size();
		mesh->m_vertexBufferSize = vertices->size() * sizeof(Vertex);

		//	
		mesh->m_indexBuffer = &(*indices)[1];
		mesh->m_indexCount = indices->size();
		mesh->m_indexBufferSize = indices->size() * sizeof(uint32_t);
		mesh->m_indiceType = UINT64_C(0x0001000000000000);

		return mesh;
	}

}
