#include "../Public/EntropyCore.h"
#include "Camera.h"

EntropyCore::EntropyCore()
{
	auto camera = new Camera();
	camera->perspective(0, 0, 0, 0);
}

Renderer* EntropyCore::CreateRenderer(HWND hwnd) {
	return RendererFactory::CreateRenderer(renderAPI, hwnd);
}
