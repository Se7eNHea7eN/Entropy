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

		std::vector<Matrix4f> m_Transforms;

	public:
		std::shared_ptr<bimg::ImageContainer> m_pImage;

		Texture(std::string& name) : Object(), m_Name(name) {};
		Texture(std::shared_ptr<bimg::ImageContainer>& image) : m_pImage(image) {};
		Texture(bimg::ImageContainer* image) : m_pImage(image) {};
		Texture(Texture&) = default;
		Texture(Texture&&) = default;
	
		void AddTransform(Matrix4f& matrix) { m_Transforms.push_back(matrix); };
		void SetName(const std::string & name) { m_Name = name; };
		void SetName(std::string && name) { m_Name = std::move(name); };
	};
}
