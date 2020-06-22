#pragma once
#include <cstdint>
#include <Eigen/Core>
#include <vector>
#include "Image.hpp"
#include "Common/Object.hpp"

namespace Entropy  {
	using namespace Eigen;
	class Texture2D : Object {
	protected:
		std::string m_Name;

		void* m_data;
		uint32_t m_size;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_depth;
		uint8_t  m_numMips;

	public:

		Texture2D(const std::string& name) : Object(), m_Name(name) {};
		Texture2D(const Texture2D&) = default;
		Texture2D(Texture2D&&) = default;
	
		void SetName(const std::string & name) { m_Name = name; };
		void SetName(std::string && name) { m_Name = std::move(name); };
	};
}
