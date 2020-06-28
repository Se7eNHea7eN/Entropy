#pragma once

#include "Renderer/RHITexture.hpp"
#include "Renderer/RHITexture2D.hpp"

#include <d3d11_1.h>
#include <wrl/client.h>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

namespace Entropy {
	class Texture2D;
	class D3D11RHITexture2D : public RHITexture2D {

	private:

	public:
		D3D11RHITexture2D(const Texture2D& texture);
		ComPtr<ID3D11ShaderResourceView> m_pTexture;

	};
}