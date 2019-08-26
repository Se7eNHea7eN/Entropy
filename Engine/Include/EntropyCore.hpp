#pragma once
#include "Common.hpp"
#include "RendererFactory.hpp"

namespace Entropy {
	class EntropyCore {
	public:
		EntropyCore();
		Renderer* CreateRenderer(HWND hwnd);
	private:
		RenderAPI renderAPI = OpenGL;
	};
}
