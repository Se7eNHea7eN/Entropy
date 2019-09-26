#pragma once

namespace Entropy {
	struct Vertex
	{
		float m_x = 0;
		float m_y = 0;
		float m_z = 0;

		float m_nx = 0;
		float m_ny = 0;
		float m_nz = 0;
		
		float m_tx = 0;
		float m_ty = 0;

		Vertex() = default;

		Vertex(float x, float y, float z, float nx, float ny, float nz)
			: m_x(x),
			  m_y(y),
			  m_z(z),
			  m_nx(nx),
			  m_ny(ny),
			  m_nz(nz) {
		}

		Vertex(float x, float y, float z, float nx, float ny, float nz, float tx, float ty)
			: m_x(x),
			  m_y(y),
			  m_z(z),
			  m_nx(nx),
			  m_ny(ny),
			  m_nz(nz),
			  m_tx(tx),
			  m_ty(ty) {
		}
	};


}
