#include "Resource\TextureManager.hpp"
#include "Renderer\RHITexture2D.hpp"

#include "Core/EntropyCore.hpp"
#include "Renderer\Renderer.hpp"
#include "Resource/Texture.hpp"

#include "..\..\ThirdParty\stb_image\stb_image.h"

using namespace Entropy;

std::shared_ptr<Entropy::RHITexture> Entropy::TextureManager::GetTexture(std::string name)
{
	auto iter = m_textureMap.find(name);

	if (m_textureMap.end() != iter) {
		return iter->second;
	}

	int iw, ih, n;
	unsigned char* idata = stbi_load(name.c_str(), &iw, &ih, &n, 0);
	Texture* pTexture = new Texture(name);
	pTexture->SetData(idata);
	auto renderer = EntropyCore::GetInstance()->GetRenderer();
	auto pRHITexutre = renderer->CreateRHITexture2D(pTexture);
	delete pTexture;
	return pRHITexutre;
}
