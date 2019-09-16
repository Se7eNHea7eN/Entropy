#pragma
#include <cstdint>
#include "Common/Object.hpp"

namespace Entropy {
	class Mesh : Object {
	public:
		Mesh()
			: m_vertexBuffer(0)
			, m_vertexBufferSize(0)
			, m_vertexCount(0)
			, m_vertexAttributeCount(0)
			// , m_indexBuffer(0)
			// , m_indexBufferSize(0)
			// , m_indexCount(0)
		{}

		void* m_vertexBuffer;
		uint32_t m_vertexBufferSize;
		uint32_t m_vertexCount;
		uint32_t m_vertexAttributeCount;
		// void* m_indexBuffer;
		// uint32_t m_indexBufferSize;
		// uint32_t m_indexCount;
	};
}
