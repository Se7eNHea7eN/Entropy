#pragma once
#include "../Private/RendererFactory.h"
class EntropyCore {
public:
	EntropyCore();
	Renderer* CreateRenderer(HWND hwnd);
private:
	RenderAPI renderAPI = OpenGL;
};