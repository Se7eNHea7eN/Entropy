#include "RendererFactory.hpp"
#include "GLRenderer.hpp"
#include "Direct3DRenderer.hpp"
using namespace Entropy;

Renderer* RendererFactory::CreateRenderer(RenderAPI renderAPI, HWND hwnd) {
	switch(renderAPI) {

	case OpenGL:
		return new GLRenderer(hwnd);
		break;
	case Direct3D:
		return new Direct3DRenderer(hwnd);
		break;
	case Vulkan: 
		break;
	default: ;
	}
	return nullptr;
}
