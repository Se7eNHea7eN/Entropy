#pragma once
#include <cstdint>
#include <Eigen/Core>
#include <vector>
#include "Image.hpp"
#include "Common/Object.hpp"

namespace bimg {
	class ImageContainer;
}

namespace Entropy  {
	using namespace Eigen;
	class Texture :Object {
	protected:
		std::string m_Name;

		uint32_t m_nTexCoordIndex;
		std::vector<Matrix4f> m_Transforms;

	public:
		bimg::ImageContainer* m_pImage;

		Texture(std::string& name) : Object(), m_Name(name), m_nTexCoordIndex(0) {};
		Texture(uint32_t coord_index, bimg::ImageContainer* image) : m_nTexCoordIndex(coord_index), m_pImage(image) {};
		Texture(Texture&) = default;
		Texture(Texture&&) = default;
	
		void AddTransform(Matrix4f& matrix) { m_Transforms.push_back(matrix); };
		void SetName(const std::string & name) { m_Name = name; };
		void SetName(std::string && name) { m_Name = std::move(name); };
	};
}
