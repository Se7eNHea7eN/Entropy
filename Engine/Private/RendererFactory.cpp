#include "RendererFactory.h"
#include "../../GLRenderer/Public/GLRenderer.h"
#include "../../Direct3DRenderer/Public/Direct3DRenderer.h"

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
