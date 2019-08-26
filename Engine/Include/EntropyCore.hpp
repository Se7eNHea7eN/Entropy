#pragma once
#include "RendererFactory.hpp"
class EntropyCore {
public:
	EntropyCore();
	Renderer* CreateRenderer(HWND hwnd);
private:
	RenderAPI renderAPI = OpenGL;
};