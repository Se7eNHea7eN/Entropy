#pragma once
#include <memory>

namespace Entropy {
	class Texture2D;
	class SkyBox {
	public:
		SkyBox() = default;
		~SkyBox() = default;
	protected:
		std::shared_ptr<Texture2D> hdrTexture;
	public:
		std::shared_ptr<Texture2D> HdrTexture() const {
			return hdrTexture;
		}

		void SetHdrTexture(const std::shared_ptr<Texture2D>& hdr_texture) {
			hdrTexture = hdr_texture;
		}
	
	};
}
