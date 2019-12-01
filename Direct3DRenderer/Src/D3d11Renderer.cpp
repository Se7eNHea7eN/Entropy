#include "D3d11Renderer.hpp"
#include "d3dUtil.h"
#include "DXTrace.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
using namespace Entropy;

D3d11Renderer::D3d11Renderer(HWND hwnd) : hwnd(hwnd) {
	RECT rc;
	GetClientRect(hwnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
}

template <class T>
inline void SafeRelease(T** ppInterfaceToRelease) {
	if (*ppInterfaceToRelease != nullptr)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = nullptr;
	}
}

D3d11Renderer::~D3d11Renderer() {
}

void D3d11Renderer::Initialize() {
	HRESULT hr = S_OK;
	if (g_pSwapchain == nullptr) {
		// create a struct to hold information about the swap chain
		DXGI_SWAP_CHAIN_DESC scd;

		// clear out the struct for use
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		// fill the swap chain description struct
		scd.BufferCount = 1; // one back buffer
		scd.BufferDesc.Width = width;
		scd.BufferDesc.Height = height;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // how swap chain is to be used
		scd.OutputWindow = hwnd; // the window to be used
		scd.SampleDesc.Count = 4; // how many multisamples
		scd.Windowed = TRUE; // windowed/full-screen mode
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching

		const D3D_FEATURE_LEVEL FeatureLevels[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};
		D3D_FEATURE_LEVEL FeatureLevelSupported;

		HRESULT hr = S_OK;

		// create a device, device context and swap chain using the information in the scd struct
		hr = D3D11CreateDeviceAndSwapChain(NULL,
		                                   D3D_DRIVER_TYPE_HARDWARE,
		                                   NULL,
		                                   0,
		                                   FeatureLevels,
		                                   _countof(FeatureLevels),
		                                   D3D11_SDK_VERSION,
		                                   &scd,
		                                   &g_pSwapchain,
		                                   &g_pDev,
		                                   &FeatureLevelSupported,
		                                   &g_pDevcon);

		if (hr == E_INVALIDARG) {
			hr = D3D11CreateDeviceAndSwapChain(NULL,
			                                   D3D_DRIVER_TYPE_HARDWARE,
			                                   NULL,
			                                   0,
			                                   &FeatureLevelSupported,
			                                   1,
			                                   D3D11_SDK_VERSION,
			                                   &scd,
			                                   &g_pSwapchain,
			                                   &g_pDev,
			                                   NULL,
			                                   &g_pDevcon);
		}

		if (hr == S_OK) {
			CreateRenderTarget();
			SetViewPort();
			InitPipeline();
			InitGraphics();
		}
	}
}

void D3d11Renderer::Resize(int w, int h) {
	width = w;
	height = h;
	//g_pSwapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

}


void D3d11Renderer::CreateRenderTarget() {
	 HRESULT hr;
     ID3D11Texture2D *pBackBuffer;
  
     // Get a pointer to the back buffer
     g_pSwapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),
                                  ( LPVOID* )&pBackBuffer );
  
     // Create a render-target view
     g_pDev->CreateRenderTargetView( pBackBuffer, nullptr,
                                           &g_pRTView );
     pBackBuffer->Release();
  
     // Bind the view
     g_pDevcon->OMSetRenderTargets( 1, &g_pRTView, nullptr );
}

void D3d11Renderer::SetViewPort() {
	D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
 
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = width;
    viewport.Height = height;
 
    g_pDevcon->RSSetViewports(1, &viewport);
}

void D3d11Renderer::InitPipeline() {

}

void D3d11Renderer::InitGraphics() {
	auto geo = std::make_shared<D3dGeometry>();
	// load and compile the two shaders
	ComPtr<ID3DBlob> VS, PS;

	HR(D3DReadFileToBlob(L"Shaders\\dx11\\vs_test.cso", VS.GetAddressOf()));
	HR(D3DReadFileToBlob(L"Shaders\\dx11\\ps_test.cso", PS.GetAddressOf()));

	// encapsulate both shaders into shader objects
	HR(g_pDev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, geo->g_pVS.GetAddressOf()));
	HR(g_pDev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, geo->g_pPS.GetAddressOf()));

	// set the shader objects
	g_pDevcon->VSSetShader(geo->g_pVS.Get(), 0, 0);
	g_pDevcon->PSSetShader(geo->g_pPS.Get(), 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"a_position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"a_color0", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	g_pDev->CreateInputLayout(ied, 2 , VS.Get()->GetBufferPointer(), VS.Get()->GetBufferSize(), geo->g_pLayout.GetAddressOf());

	g_pDevcon->IASetInputLayout(geo->g_pLayout.Get());

	VS->Release();
	PS->Release();



	// ******************
	// ���������嶥��
	//    5________ 6
	//    /|      /|
	//   /_|_____/ |
	//  1|4|_ _ 2|_|7
	//   | /     | /
	//   |/______|/
	//  0       3
    VERTEX OurVertices[] =
    {
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
    };
 
    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
 
    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof OurVertices;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
 
		// �½����㻺����
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = OurVertices;

    HR(g_pDev->CreateBuffer(&bd, &InitData, &geo->g_pVBuffer));       // create the buffer
 
	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	g_pDevcon->Map(geo->g_pVBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                       // copy the data
	g_pDevcon->Unmap(geo->g_pVBuffer.Get(), NULL);                                      // unmap the buffer



	// ******************
	// ��������
	//
	WORD indices[] = {
		// ����
		0, 1, 2,
		2, 3, 0,
		// ����
		4, 5, 1,
		1, 0, 4,
		// ����
		1, 5, 6,
		6, 2, 1,
		// ����
		7, 6, 5,
		5, 4, 7,
		// ����
		3, 2, 6,
		6, 7, 3,
		// ����
		4, 0, 3,
		3, 7, 4
	};

	// ������������������
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// �½�����������
	InitData.pSysMem = indices;
	HR(g_pDev->CreateBuffer(&ibd, &InitData, geo->g_pIBuffer.GetAddressOf()));
	// ����װ��׶ε���������������
	g_pDevcon->IASetIndexBuffer(geo->g_pIBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);


	// ******************
	// ���ó�������������
	//
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(geo->g_pCBuffer.Get());
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// �½���������������ʹ�ó�ʼ����
	HR(g_pDev->CreateBuffer(&cbd, nullptr, geo->g_pCBuffer.GetAddressOf()));


	geometries.push_back(geo);
}

void D3d11Renderer::Draw() {
	Initialize();
	const FLOAT clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	g_pDevcon->ClearRenderTargetView(g_pRTView, clearColor);
	for(auto geo : geometries)
	// do 3D rendering on the back buffer here
	{
		// select which vertex buffer to display
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		g_pDevcon->IASetInputLayout(geo->g_pLayout.Get());

		g_pDevcon->IASetVertexBuffers(0, 1, geo->g_pVBuffer.GetAddressOf(), &stride, &offset);

		//// select which primtive type we are using
		g_pDevcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//// draw the vertex buffer to the back buffer
		g_pDevcon->Draw(8, 0);
	}

	// swap the back buffer and the front buffer
	g_pSwapchain->Present(0, 0);

}

void D3d11Renderer::AwaitRenderFrame() {
}
