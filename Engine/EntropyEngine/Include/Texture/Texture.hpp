#pragma once
#include <cstdint>
#include <Eigen/Core>
#include <vector>
#include "Image.hpp"
#include "Common/Object.hpp"

namespace Entropy  {
	using namespace Eigen;
	class Texture {
	protected:
		std::string m_Name;

		void* m_data;
		uint32_t m_size;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_depth;
		uint8_t  m_numMips;

	public:

		Texture(const std::string& name) : m_Name(name) {};
		Texture(const Texture&) = default;
		Texture(Texture&&) = default;
	
		void SetName(const std::string & name) { m_Name = name; };
		void SetName(std::string && name) { m_Name = std::move(name); };

		void SetData(void* pData) { m_data = pData; }
	};
}
