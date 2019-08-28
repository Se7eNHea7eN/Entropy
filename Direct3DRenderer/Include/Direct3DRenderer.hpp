#pragma once
#include "Renderer.hpp"
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

namespace Entropy {
	class Direct3DRenderer : public Renderer {
	public:
		Direct3DRenderer(HWND hwnd);
		~Direct3DRenderer();
		void initialize() override;
		void resize(int w, int h) override;
		void draw() override;

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
		
		
		ID3D11InputLayout* g_pLayout = nullptr; // the pointer to the input layout
		ID3D11VertexShader* g_pVS = nullptr; // the pointer to the vertex shader
		ID3D11PixelShader* g_pPS = nullptr; // the pointer to the pixel shader
		
		ID3D11Buffer* g_pVBuffer = nullptr; // Vertex Buffer
		
	
		struct VERTEX {
			XMFLOAT3 Position;
			XMFLOAT4 Color;
		};

	};
}
