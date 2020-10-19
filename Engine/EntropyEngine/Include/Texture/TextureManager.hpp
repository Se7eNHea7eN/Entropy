#pragma once
#include <map>
#include <memory>
#include <string>
namespace Entropy {
	class RHITexture;
	class TextureManager {
	public:
		static TextureManager* GetInstance() {
			if (s_pTextureManager == nullptr) {
				s_pTextureManager = new TextureManager();
			}
			return s_pTextureManager;
		}

		std::shared_ptr<RHITexture> GetTexture(std::string name);

	private:
		TextureManager() {
		}
		static TextureManager* s_pTextureManager;
		std::map<std::string, std::shared_ptr<RHITexture>> m_textureMap;
	};
}
