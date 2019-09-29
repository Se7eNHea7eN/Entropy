#pragma once
#include <memory>

namespace Entropy {
	class Texture;
	class SkyBox {
	public:
		SkyBox() = default;
		~SkyBox() = default;
	protected:
		std::shared_ptr<Texture> hdrTexture;
	public:
		std::shared_ptr<Texture> HdrTexture() const {
			return hdrTexture;
		}

		void SetHdrTexture(const std::shared_ptr<Texture>& hdr_texture) {
			hdrTexture = hdr_texture;
		}
	
	};
}
