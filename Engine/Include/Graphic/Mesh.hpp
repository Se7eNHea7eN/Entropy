#pragma once
#include <cstdint>
#include "Common/Object.hpp"

namespace Entropy {
	class Mesh : Object {
	public:
		Mesh()
			: m_vertexBuffer(nullptr)
			, m_vertexBufferSize(0)
			, m_vertexCount(0)
			, m_vertexAttributeCount(0)
			, m_indexBuffer(nullptr)
			, m_indexBufferSize(0)
			, m_indexCount(0)
			, m_materialIndex (0)
		{}

		const void* m_vertexBuffer;
		uint32_t m_vertexBufferSize;
		uint32_t m_vertexCount;
		uint32_t m_vertexAttributeCount;
		const void* m_indexBuffer;
		uint32_t m_indexBufferSize;
		uint32_t m_indexCount;
		uint32_t m_materialIndex;
	};
}
