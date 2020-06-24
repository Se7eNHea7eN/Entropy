#include "D3d11Renderer.hpp"
#include "d3dUtil.h"
#include "DXTrace.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Renderer/RHITexture2D.hpp"

#include "Resource\D3D11RHITexture2D.hpp"

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

	HR(D3DReadFileToBlob(L"Shaders\\dx11\\Cube_VS.cso", VS.GetAddressOf()));
	HR(D3DReadFileToBlob(L"Shaders\\dx11\\Cube_PS.cso", PS.GetAddressOf()));

	// encapsulate both shaders into shader objects
	HR(g_pDev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), nullptr, geo->g_pVS.GetAddressOf()));
	HR(g_pDev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), nullptr, geo->g_pPS.GetAddressOf()));

	// set the shader objects
	g_pDevcon->VSSetShader(geo->g_pVS.Get(), 0, 0);
	g_pDevcon->PSSetShader(geo->g_pPS.Get(), 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HR(g_pDev->CreateInputLayout(ied, 2 , VS->GetBufferPointer(), VS->GetBufferSize(), geo->g_pLayout.GetAddressOf()));

	g_pDevcon->IASetInputLayout(geo->g_pLayout.Get());

	VS->Release();
	PS->Release();



	// ******************
	// 设置立方体顶点
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
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
 
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof OurVertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
 
		// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = OurVertices;

    HR(g_pDev->CreateBuffer(&vbd, &InitData, &geo->g_pVBuffer));       // create the buffer

	// ******************
	// 索引数组
	//
	WORD indices[] = {
		// 正面
		0, 1, 2,
		2, 3, 0,
		// 左面
		4, 5, 1,
		1, 0, 4,
		// 顶面
		1, 5, 6,
		6, 2, 1,
		// 背面
		7, 6, 5,
		5, 4, 7,
		// 右面
		3, 2, 6,
		6, 7, 3,
		// 底面
		4, 0, 3,
		3, 7, 4
	};

	// 设置索引缓冲区描述
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = indices;
	HR(g_pDev->CreateBuffer(&ibd, &InitData, geo->g_pIBuffer.GetAddressOf()));
	// 输入装配阶段的索引缓冲区设置
	g_pDevcon->IASetIndexBuffer(geo->g_pIBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);


	// ******************
	// 设置常量缓冲区描述
	//
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 新建常量缓冲区，不使用初始数据
	HR(g_pDev->CreateBuffer(&cbd, nullptr, geo->g_pCBuffer.GetAddressOf()));

	// 初始化常量缓冲区的值
	m_CBuffer.world = XMMatrixIdentity();	// 单位矩阵的转置是它本身
	m_CBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_CBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.0* width/height, 1.0f, 1000.0f));

	// ******************
	// 给渲染管线各个阶段绑定好所需资源
	//

	// 输入装配阶段的顶点缓冲区设置
	UINT stride = sizeof(VERTEX);	// 跨越字节数
	UINT offset = 0;						// 起始偏移量

	g_pDevcon->IASetVertexBuffers(0, 1, geo->g_pVBuffer.GetAddressOf(), &stride, &offset);
	// 设置图元类型，设定输入布局
	g_pDevcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pDevcon->IASetInputLayout(geo->g_pLayout.Get());
	// 将着色器绑定到渲染管线
	g_pDevcon->VSSetShader(geo->g_pVS.Get(), nullptr, 0);
	// 将更新好的常量缓冲区绑定到顶点着色器
	g_pDevcon->VSSetConstantBuffers(0, 1, geo->g_pCBuffer.GetAddressOf());

	g_pDevcon->PSSetShader(geo->g_pPS.Get(), nullptr, 0);
	geometries.push_back(geo);
}

std::shared_ptr<RHITexture2D> Entropy::D3d11Renderer::CreateRHITexture2D()
{
	auto texture2D = std::shared_ptr<D3D11RHITexture2D>();

	return texture2D;
}

void D3d11Renderer::Draw() {
	Initialize();
	const FLOAT clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	g_pDevcon->ClearRenderTargetView(g_pRTView, clearColor);


	static float phi = 0.0f, theta = 0.0f;
	phi += 0.0001f, theta += 0.00015f;
	m_CBuffer.world = XMMatrixTranspose(XMMatrixRotationX(phi) * XMMatrixRotationY(theta));


	for(auto geo : geometries)
	// do 3D rendering on the back buffer here
	{

		// 更新常量缓冲区，让立方体转起来
		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(g_pDevcon->Map(geo->g_pCBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
		memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer));

		g_pDevcon->Unmap(geo->g_pCBuffer.Get(), 0);



		// select which vertex buffer to display
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		g_pDevcon->IASetInputLayout(geo->g_pLayout.Get());

		g_pDevcon->IASetVertexBuffers(0, 1, geo->g_pVBuffer.GetAddressOf(), &stride, &offset);

		//// select which primtive type we are using
		g_pDevcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//// draw the vertex buffer to the back buffer
		g_pDevcon->DrawIndexed(36, 0,0);
	}

	// swap the back buffer and the front buffer
	g_pSwapchain->Present(0, 0);

}

void D3d11Renderer::AwaitRenderFrame() {
}
