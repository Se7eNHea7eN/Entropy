#include "Common/RendererFactory.hpp"
#include "GLRenderer.hpp"
#include "Direct3DRenderer.hpp"
#include "BgfxRenderer.hpp"

using namespace Entropy;

Renderer* RendererFactory::CreateRenderer(RenderAPI renderAPI, HWND hwnd) {
	switch(renderAPI) {
	case Bgfx:
		return new BgfxRenderer(hwnd);
	case OpenGL:
		return new GLRenderer(hwnd);
	case Direct3D:
		return new Direct3DRenderer(hwnd);
	case Vulkan: 
		break;
	default: ;
	}
	return nullptr;
}
