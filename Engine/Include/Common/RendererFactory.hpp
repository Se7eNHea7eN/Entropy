#pragma once
#include "windows.h"

namespace Entropy {
	enum RenderAPI {
		Bgfx = 0,
		OpenGL = 1,
		Direct3D = 2,
		Vulkan = 3
	};
	class Renderer;
	class RendererFactory {
	public:
		static Renderer* CreateRenderer(RenderAPI renderAPI, HWND hwnd);
	};
}
