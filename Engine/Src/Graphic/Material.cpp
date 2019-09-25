#include "Graphic/Material.hpp"
#include <memory>
#include <bimg/bimg.h>
bimg::ImageContainer* Entropy::Material::buildImageByFloat(float value) {
	return bimg::imageAlloc(getAllocator()
		, bimg::TextureFormat::R8
		, uint16_t(1)
		, uint16_t(1)
		, 0
		, 1
		, false
		, false
		, new uint8_t(value * 255)
	);
}
