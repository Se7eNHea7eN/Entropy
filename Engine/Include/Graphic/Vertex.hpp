#pragma once

namespace Entropy {
	struct Vertex
	{
		float x = 0;
		float y = 0;
		float z = 0;

		float normalX = 0;
		float normalY = 0;
		float normalZ = 0;
		
		float texCoordX = 0;
		float texCoordY = 0;

		float tangentX = 0;
		float tangentY = 0;
		float tangentZ = 0;

		Vertex() = default;

		Vertex(float x, float y, float z, float nx, float ny, float nz)
			: x(x),
			  y(y),
			  z(z),
			  normalX(nx),
			  normalY(ny),
			  normalZ(nz) {
		}

		Vertex(float x, float y, float z, float nx, float ny, float nz, float tx, float ty)
			: x(x),
			  y(y),
			  z(z),
			  normalX(nx),
			  normalY(ny),
			  normalZ(nz),
			  texCoordX(tx),
			  texCoordY(ty) {
		}

		Vertex(float x, float y, float z, float normal_x, float normal_y, float normal_z, float tex_coord_x, float tex_coord_y, float tangent_x, float tangent_y, float tangent_z, float bitangent_x, float bitangent_y, float bitangent_z)
			: x(x),
			y(y),
			z(z),
			normalX(normal_x),
			normalY(normal_y),
			normalZ(normal_z),
			texCoordX(tex_coord_x),
			texCoordY(tex_coord_y),
			tangentX(tangent_x),
			tangentY(tangent_y),
			tangentZ(tangent_z) {
		}

	};


}
