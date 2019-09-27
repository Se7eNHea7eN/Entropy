#pragma once
#include <cstdint>
#include "Common/Object.hpp"
#include "Graphic/Vertex.hpp"
namespace Entropy {
	class Mesh : Object {
	public:
		Mesh() = default;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		uint16_t m_materialIndex = 0;
		uint64_t m_indiceType = 0;

		void CalculateTangents() {
			for (unsigned int i = 0; i < vertices.size(); i += 3) {
				Vertex& v0 = vertices[i];
				Vertex& v1 = vertices[i + 1];
				Vertex& v2 = vertices[i + 2];
				float Edge1X = v1.x - v0.x;
				float Edge1Y = v1.y - v0.y;
				float Edge1Z = v1.z - v0.z;
				float Edge2X = v2.x - v0.x;
				float Edge2Y = v2.y - v0.y;
				float Edge2Z = v2.z - v0.z;

				float DeltaU1 = v1.texCoordX - v0.texCoordX;
				float DeltaV1 = v1.texCoordY - v0.texCoordY;
				float DeltaU2 = v2.texCoordX - v0.texCoordX;
				float DeltaV2 = v2.texCoordY - v0.texCoordY;
				float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);
				float tangentX = f * (DeltaV2 * Edge1X - DeltaV1 * Edge2X);
				float tangentY = f * (DeltaV2 * Edge1Y - DeltaV1 * Edge2Y);
				float tangentZ = f * (DeltaV2 * Edge1Z - DeltaV1 * Edge2Z);

				v0.tangentX = tangentX;
				v0.tangentY = tangentY;
				v0.tangentZ = tangentZ;
				v1.tangentX = tangentX;
				v1.tangentY = tangentY;
				v1.tangentZ = tangentZ;
				v2.tangentX = tangentX;
				v2.tangentY = tangentY;
				v2.tangentZ = tangentZ;
			}
		}
	};
}
