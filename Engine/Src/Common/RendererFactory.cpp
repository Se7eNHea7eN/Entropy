#include "Common/RendererFactory.hpp"
#include "GLRenderer.hpp"
#include "D3d11Renderer.hpp"
#include "BgfxRenderer.hpp"

using namespace Entropy;

Renderer* RendererFactory::CreateRenderer(RenderAPI renderAPI, HWND hwnd) {
	switch(renderAPI) {
	case Bgfx:
		return new BgfxRenderer(hwnd);
	case OpenGL:
		return new GLRenderer(hwnd);
	case D3d11:
		return new D3d11Renderer(hwnd);
	case Vulkan: 
		break;
	default: ;
	}
	return nullptr;
}
