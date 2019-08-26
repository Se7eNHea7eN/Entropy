#pragma once
#include "windows.h"
#include "Renderer.hpp"

enum RenderAPI {
	OpenGL = 1,
	Direct3D = 2,
	Vulkan = 3
};

class RendererFactory {
public:
	static Renderer* CreateRenderer(RenderAPI renderAPI, HWND hwnd);
};

