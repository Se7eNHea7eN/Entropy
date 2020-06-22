#pragma once
#include <cstdint>
#include <vector>
#include "Common/Object.hpp"
#include "Graphic/Vertex.hpp"
#include "Eigen/Core"
using namespace Eigen;
namespace Entropy {
	class Mesh : Object {
	public:
		Mesh() = default;
		Mesh( Mesh& m) = default;

		~Mesh() = default;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		uint16_t m_materialIndex = 0;
		uint64_t m_indiceType = 0;

		void CalculateTangents() {
			if(indices.empty())
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

					Vector3f tangent(
						f * (DeltaV2 * Edge1X - DeltaV1 * Edge2X),
						f * (DeltaV2 * Edge1Y - DeltaV1 * Edge2Y),
						f * (DeltaV2 * Edge1Z - DeltaV1 * Edge2Z)
					);
					tangent.normalize();

					v0.tangentX = tangent.x();
					v0.tangentY = tangent.y();
					v0.tangentZ = tangent.z();
					v1.tangentX = tangent.x();
					v1.tangentY = tangent.y();
					v1.tangentZ = tangent.z();
					v2.tangentX = tangent.x();
					v2.tangentY = tangent.y();
					v2.tangentZ = tangent.z();
				}
			else
				for (unsigned int i = 0; i < indices.size(); i += 3) {
					Vertex& v0 = vertices[indices[i]];
					Vertex& v1 = vertices[indices[i + 1]];
					Vertex& v2 = vertices[indices[i + 2]];
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
					
					Vector3f tangent(
						f * (DeltaV2 * Edge1X - DeltaV1 * Edge2X),
						f * (DeltaV2 * Edge1Y - DeltaV1 * Edge2Y),
						f * (DeltaV2 * Edge1Z - DeltaV1 * Edge2Z)
					);
					tangent.normalize();

					v0.tangentX = tangent.x();
					v0.tangentY = tangent.y();
					v0.tangentZ = tangent.z();
					v1.tangentX = tangent.x();
					v1.tangentY = tangent.y();
					v1.tangentZ = tangent.z();
					v2.tangentX = tangent.x();
					v2.tangentY = tangent.y();
					v2.tangentZ = tangent.z();
				}
		}
	};
}
