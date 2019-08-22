#pragma once
#include "../../Engine/Public/Renderer.h"
#include <d3d11.h>

class Direct3DRenderer : public Renderer
{
public:
	Direct3DRenderer(HWND hwnd);
	~Direct3DRenderer();
	void initialize() override;
	void resize(int w, int h) override;
	void draw() override;

private:

	HWND      hwnd;
	HWND      mhMainWnd;
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;

	UINT      m4xMsaaQuality;
	bool mEnable4xMsaa;
	ID3D11Device* md3dDevice;
	D3D_DRIVER_TYPE md3dDriverType;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;
};
