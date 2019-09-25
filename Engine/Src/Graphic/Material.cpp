#include "Graphic/Material.hpp"

std::shared_ptr<bimg::ImageContainer> Entropy::buildImageByFloat(float value) {
	auto image = std::make_shared<bimg::ImageContainer>();
	image->m_data = new float(value);
	image->m_size = sizeof(ColorRGBA);
	image->m_width = 1;
	image->m_height = 1;
	image->m_depth = sizeof(ColorRGBA);
	image->m_numLayers = 1;
	image->m_numMips = 0;
	image->m_hasAlpha = true;
	image->m_format = bimg::TextureFormat::RGBA8;
	return image;
}
