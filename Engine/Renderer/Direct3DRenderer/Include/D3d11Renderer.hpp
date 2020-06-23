#pragma once
#include "Renderer/Renderer.hpp"
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <list>
#include <memory>
#include <wrl\client.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace Entropy {
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	class D3dGeometry {
	public:
		ComPtr<ID3D11InputLayout> g_pLayout; // the pointer to the input layout
		ComPtr<ID3D11VertexShader> g_pVS; // the pointer to the vertex shader
		ComPtr<ID3D11PixelShader> g_pPS; // the pointer to the pixel shader

		ComPtr<ID3D11Buffer> g_pVBuffer; // Vertex Buffer
		ComPtr<ID3D11Buffer> g_pIBuffer; // Index Buffer
		ComPtr<ID3D11Buffer> g_pCBuffer; // Constant Buffer

	};


	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	class D3d11Renderer : public Renderer {
	public:
		D3d11Renderer(HWND hwnd);
		~D3d11Renderer();
		void Initialize() override;
		void Resize(int w, int h) override;
		void Draw() override;
		void AwaitRenderFrame() override;

	private:
		void CreateRenderTarget();
		void SetViewPort();
		void InitPipeline();
		void InitGraphics();
		HWND hwnd;
		
		IDXGISwapChain* g_pSwapchain = nullptr; // the pointer to the swap chain interface
		
		ID3D11Device* g_pDev = nullptr; // the pointer to our Direct3D device interface
		
		ID3D11DeviceContext* g_pDevcon = nullptr; // the pointer to our Direct3D device context
		
		ID3D11RenderTargetView* g_pRTView = nullptr;
		

		struct VERTEX {
			XMFLOAT3 Position;
			XMFLOAT4 Color;
		};

		std::list<std::shared_ptr<D3dGeometry>> geometries;
		ConstantBuffer m_CBuffer;	                    // 用于修改GPU常量缓冲区的变量
	};


}
